#pragma once

#include "dto/ErrorsDtos.hpp"
#include "dto/BonusDtos.hpp"
#include "dto/FlightDtos.hpp"
#include <dto/TicketDtos.hpp>
#include <dto/GatewayDtos.h>
#include <dto/IdentityProviderDtos.hpp>

#include "service/BonusService.hpp"
#include "service/FlightService.hpp"
#include "service/TicketService.hpp"
#include "service/StatisticsService.hpp"
#include "service/IdentityProviderService.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "producer/IProducer.h"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "JWTAuth.h"
#include <iostream>

#include OATPP_CODEGEN_BEGIN(ApiController) //<--- codegen begin

class GatewayController : public oatpp::web::server::api::ApiController {

protected:

    GatewayController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
    {}

public:
    static std::shared_ptr<BaseConfig> config;
    static std::shared_ptr<IProducer> producer;
    static std::shared_ptr<StatisticsService> statisticsService;
    static std::shared_ptr<BonusService> bonusService;
    static std::shared_ptr<FlightService> flightService;
    static std::shared_ptr<TicketService> ticketService;
    static std::shared_ptr<IdentityProviderService> identityProviderService;

    static std::shared_ptr<GatewayController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                           objectMapper)) {
        return std::shared_ptr<GatewayController>(new GatewayController(objectMapper));
    }

 ENDPOINT_ASYNC("GET", "/api/v1/statistics", StatisticsGetPoint ) {

    ENDPOINT_ASYNC_INIT(StatisticsGetPoint)
        Action act() override {
            std::string begin_time = request->getQueryParameter("begin_time");
            std::string end_time = request->getQueryParameter("end_time");
            auto response = statisticsService->StatisticsGetPoint(begin_time, end_time);
            if (response->getStatusCode() != 200)
            {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                return _return(resp);
            }
            auto stats = response->readBodyToDto<oatpp::Object<StatisticsResponseDto>>(
                    controller->getDefaultObjectMapper());
            auto resp = controller->createDtoResponse(Status::CODE_200, stats);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

            return _return(resp);
        }

    };

    ENDPOINT_ASYNC("POST", "/api/v1/authorize", AuthPoint ) {
    ENDPOINT_ASYNC_INIT(AuthPoint)
        Action act() override {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

            auto dto = AuthResponseDto::createShared();
            dto->client_id = config->GetIntField( { ClientIDSection });
            dto->auth_url = std::string(config->GetStringField( { AuthURLSection }));
            dto->scope_string = std::string(config->GetStringField( { ScopeStringSection }));
            
            auto resp = controller->createDtoResponse(Status::CODE_200, dto);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/authorize",  "POST", std::to_string(startTime), std::to_string(finishTime), std::to_string(finishTime - startTime), "unknown", 200);
            producer->PostToBroker(s);
            return _return(resp);
        }
    };

    ENDPOINT_ASYNC("POST", "/api/v1/callback", CallbackPoint ) {
    ENDPOINT_ASYNC_INIT(CallbackPoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&CallbackPoint::Response);
        }
        Action Response( const oatpp::String& str) {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            oatpp::Object<CallbackRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<CallbackRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_400, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/callback",  "POST", std::to_string(startTime), std::to_string(finishTime), std::to_string(finishTime - startTime), "unknown", 400);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto dto = TokenRequestDto::createShared();

            dto->client_secret = std::string(config->GetStringField( { ClientSecretSection }));
            dto->auth_code = body->auth_code;
            

            auto response = identityProviderService->TokenGetPoint(dto);

            if (response->getStatusCode() != 200)
            {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/callback",  "POST", std::to_string(startTime), std::to_string(finishTime), std::to_string(finishTime - startTime), "unknown", 500);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto tokenResp = response->readBodyToDto<oatpp::Object<TokenResponseDto>>(
                    controller->getDefaultObjectMapper());
            auto resp = controller->createDtoResponse(Status::CODE_200, tokenResp);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/callback",  "POST", std::to_string(startTime), std::to_string(finishTime), std::to_string(finishTime - startTime), "unknown", 200);
            producer->PostToBroker(s);
            return _return(resp);
        }
    };

    ENDPOINT_ASYNC("GET", "/api/v1/flights", FlightsGetPoint) {
    ENDPOINT_ASYNC_INIT(FlightsGetPoint)

        Action act() override {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            int page = std::stoi(request->getQueryParameter("page"));
            int pageSize = std::stoi(request->getQueryParameter("size"));
            if (page < 1){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong page");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_400, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                std::string username_log = "unknown";
                auto auth_token = request->getHeader("Authorization");
                if (auth_token) {
                    JWTAuth auth(config);
                    auto token = auth.extractToken(auth_token);
                    if (auth.checkToken(token))
                        username_log = auth.getLogin(token);
                }

                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/flights?page=" + std::to_string(page) + "size=" + std::to_string(pageSize),  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            400);
                producer->PostToBroker(s);
                return _return(resp);
            }

            
            if (pageSize < 1 || pageSize > 100){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong page size");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_400, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto auth_token = request->getHeader("Authorization");
                if (auth_token) {
                    JWTAuth auth(config);
                    auto token = auth.extractToken(auth_token);
                    if (auth.checkToken(token))
                        username_log = auth.getLogin(token);
                }

                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/flights?page=" + std::to_string(page) + "size=" + std::to_string(pageSize),  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            400);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto response = flightService->FlightsGetPoint(page, pageSize);

            if (response->getStatusCode() != 200)
            {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto auth_token = request->getHeader("Authorization");
                if (auth_token) {
                    JWTAuth auth(config);
                    auto token = auth.extractToken(auth_token);
                    if (auth.checkToken(token))
                        username_log = auth.getLogin(token);
                }

                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/flights?page=" + std::to_string(page) + "size=" + std::to_string(pageSize),  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto flights = response->readBodyToDto<oatpp::Object<FlightsResponseDto>>(
                    controller->getDefaultObjectMapper());
            auto resp = controller->createDtoResponse(Status::CODE_200, flights);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            std::string username_log = "unknown";
            auto auth_token = request->getHeader("Authorization");
            if (auth_token) {
                JWTAuth auth(config);
                auto token = auth.extractToken(auth_token);
                if (auth.checkToken(token))
                    username_log = auth.getLogin(token);
            }

            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/flights?page=" + std::to_string(page) + "size=" + std::to_string(pageSize),  
                        "GET", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        username_log, 
                        200);
            producer->PostToBroker(s);
            return _return(resp);
        }

    };

 ENDPOINT_ASYNC("GET", "/api/v1/flights/{flight_number}", FlightGetPoint) {
    ENDPOINT_ASYNC_INIT(FlightGetPoint)

        Action act() override {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            std::string flight_number = request->getPathVariable("flight_number");
            if (flight_number.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no flight number provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_400, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto auth_token = request->getHeader("Authorization");
                if (auth_token) {
                    JWTAuth auth(config);
                    auto token = auth.extractToken(auth_token);
                    if (auth.checkToken(token))
                        username_log = auth.getLogin(token);
                }

                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/flights/" + flight_number,  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            400);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto response = flightService->FlightGetPoint(flight_number);

            if (response->getStatusCode() != 200)
            {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto auth_token = request->getHeader("Authorization");
                if (auth_token) {
                    JWTAuth auth(config);
                    auto token = auth.extractToken(auth_token);
                    if (auth.checkToken(token))
                        username_log = auth.getLogin(token);
                }

                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/flights/" + flight_number,  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto flight = response->readBodyToDto<oatpp::Object<FlightResponseDto>>(
                    controller->getDefaultObjectMapper());
            auto resp = controller->createDtoResponse(Status::CODE_200, flight);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            std::string username_log = "unknown";
            auto auth_token = request->getHeader("Authorization");
            if (auth_token) {
                JWTAuth auth(config);
                auto token = auth.extractToken(auth_token);
                if (auth.checkToken(token))
                    username_log = auth.getLogin(token);
            }

            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/flights/" + flight_number,  
                        "GET", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        username_log, 
                        200);
            producer->PostToBroker(s);
            return _return(resp);
        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/tickets/{ticketUid}", TicketGetPoint) {
    ENDPOINT_ASYNC_INIT(TicketGetPoint)
        Action act() override {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            auto auth_token = request->getHeader("Authorization");
            if (!auth_token) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no auth token provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + request->getPathVariable("ticketUid"),  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }
            JWTAuth auth(config);
            auto token = auth.extractToken(auth_token);
            if (!auth.checkToken(token))  {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong tiken provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + request->getPathVariable("ticketUid"),  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }
            
            std::string uuid = request->getPathVariable("ticketUid");
            if (uuid.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no uuid provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_400, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                
                std::string username_log = auth.getLogin(token);
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + uuid,
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            400);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto ticketResponse = ticketService->TicketGetPoint(uuid, auth_token);
            if (ticketResponse->getStatusCode() != 200)
            {
                if (ticketResponse->getStatusCode() == 404) {
                    auto resp = controller->createResponse(Status::CODE_404);
                    resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                    resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                    resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                    std::string username_log = auth.getLogin(token);
                    auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                    LogString s("/api/v1/tickets/" + uuid,
                                "GET", 
                                std::to_string(startTime), 
                                std::to_string(finishTime), 
                                std::to_string(finishTime - startTime), 
                                username_log, 
                                404);
                    producer->PostToBroker(s);
                    return _return(resp);
                }
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = auth.getLogin(token);
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + uuid,
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto ticket = ticketResponse->readBodyToDto<oatpp::Object<TicketResponseDto>>(controller->getDefaultObjectMapper());

            auto flightResponse = flightService->FlightGetPoint(ticket->flightNumber);
            if (flightResponse->getStatusCode() != 200)
            {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = auth.getLogin(token);
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + uuid,
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(controller->getDefaultObjectMapper());

            auto ticketDto = FullTicketResponseDto::createShared();
            ticketDto->flightNumber = ticket->flightNumber;
            ticketDto->ticketUid = ticket->ticketUid;
            ticketDto->status = ticket->status;
            ticketDto->price = ticket->price;
            ticketDto->date = flight->date;
            ticketDto->fromAirport = flight->fromAirport;
            ticketDto->toAirport = flight->toAirport;
            auto resp = controller->createDtoResponse(Status::CODE_200, ticketDto);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            std::string username_log = auth.getLogin(token);
            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/tickets/" + uuid,
                        "GET", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        username_log, 
                        200);
            producer->PostToBroker(s);
            return _return(resp);

        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/tickets", TicketsGetPoint) {
    ENDPOINT_ASYNC_INIT(TicketsGetPoint)
        Action act() override {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            auto auth_token = request->getHeader("Authorization");
            if (!auth_token) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no auth token provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }
            JWTAuth auth(config);
            auto token = auth.extractToken(auth_token);
            if (!auth.checkToken(token))  {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong tiken provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto ticketsResponse = ticketService->TicketsGetPoint(auth_token);
            if (ticketsResponse->getStatusCode() != 200) {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = auth.getLogin(token);
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto tickets = ticketsResponse->readBodyToDto<oatpp::Vector<oatpp::Object<TicketResponseDto>>>(
                    controller->getDefaultObjectMapper());
            oatpp::Vector<oatpp::Object<FullTicketResponseDto>> dtoVector({});
            for (const auto &t : *tickets) {
                auto flightResponse = flightService->FlightGetPoint(t->flightNumber);
                if (flightResponse->getStatusCode() != 200) {
                    auto resp = controller->createResponse(Status::CODE_500);
                    resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                    resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                    resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                    std::string username_log = auth.getLogin(token);
                    auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                    LogString s("/api/v1/tickets",  
                                "GET", 
                                std::to_string(startTime), 
                                std::to_string(finishTime), 
                                std::to_string(finishTime - startTime), 
                                username_log, 
                                500);
                    producer->PostToBroker(s);
                    return _return(resp);
                }
                auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(
                        controller->getDefaultObjectMapper());
                auto ticketDto = FullTicketResponseDto::createShared();
                ticketDto->flightNumber = t->flightNumber;
                ticketDto->ticketUid = t->ticketUid;
                ticketDto->status = t->status;
                ticketDto->price = t->price;
                ticketDto->date = flight->date;
                ticketDto->fromAirport = flight->fromAirport;
                ticketDto->toAirport = flight->toAirport;
                dtoVector->push_back(ticketDto);
            }

            auto resp = controller->createDtoResponse(Status::CODE_200, dtoVector);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            std::string username_log = auth.getLogin(token);
            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/tickets",  
                        "GET", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        username_log, 
                        200);
            producer->PostToBroker(s);
            return _return(resp);
        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/privilege", PrivilegeGetPoint) {
    ENDPOINT_ASYNC_INIT(PrivilegeGetPoint)

        Action act() override {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            auto auth_token = request->getHeader("Authorization"); 
            if (!auth_token) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no auth token provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/privilege",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }
            JWTAuth auth(config);
            auto token = auth.extractToken(auth_token);
            if (!auth.checkToken(token))  {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong tiken provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/privilege",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto bonusResponse = bonusService->BalanceGetPoint(auth_token);
            if (bonusResponse->getStatusCode() != 200) {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = auth.getLogin(token);
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/privilege",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto bonus = bonusResponse->readBodyToDto<oatpp::Object<BalanceResponseDto>>(
                    controller->getDefaultObjectMapper());

            auto resp = controller->createDtoResponse(Status::CODE_200, bonus);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            std::string username_log = auth.getLogin(token);
            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/privilege",  
                        "GET", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        username_log, 
                        200);
            producer->PostToBroker(s);
            return _return(resp);
        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/me", MeGetPoint) {
    ENDPOINT_ASYNC_INIT(MeGetPoint)

        Action act() override {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            auto auth_token = request->getHeader("Authorization"); 
            if (!auth_token) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no auth token provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/me",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }
            JWTAuth auth(config);
            auto token = auth.extractToken(auth_token);
            if (!auth.checkToken(token))  {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong tiken provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/me",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto bonusResponse = bonusService->BalanceGetPoint(auth_token);
            if (bonusResponse->getStatusCode() != 200) {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = auth.getLogin(token);
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/me",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto bonus = bonusResponse->readBodyToDto<oatpp::Object<BalanceResponseDto>>(
                    controller->getDefaultObjectMapper());

            auto ticketsResponse = ticketService->TicketsGetPoint(auth_token);
            if (ticketsResponse->getStatusCode() != 200) {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = auth.getLogin(token);
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/me",  
                            "GET", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto tickets = ticketsResponse->readBodyToDto<oatpp::Vector<oatpp::Object<TicketResponseDto>>>(
                    controller->getDefaultObjectMapper());
            oatpp::Vector<oatpp::Object<FullTicketResponseDto>> dtoVector({});
            for (const auto &t : *tickets) {
                auto flightResponse = flightService->FlightGetPoint(t->flightNumber);
                if (flightResponse->getStatusCode() != 200) {
                    auto resp = controller->createResponse(Status::CODE_500);
                    resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                    resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                    resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                    std::string username_log = auth.getLogin(token);
                    auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                    LogString s("/api/v1/me",  
                                "GET", 
                                std::to_string(startTime), 
                                std::to_string(finishTime), 
                                std::to_string(finishTime - startTime), 
                                username_log, 
                                500);
                    producer->PostToBroker(s);
                    return _return(resp);
                }
                auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(
                        controller->getDefaultObjectMapper());
                auto ticketDto = FullTicketResponseDto::createShared();
                ticketDto->flightNumber = t->flightNumber;
                ticketDto->ticketUid = t->ticketUid;
                ticketDto->status = t->status;
                ticketDto->price = t->price;
                ticketDto->date = flight->date;
                ticketDto->fromAirport = flight->fromAirport;
                ticketDto->toAirport = flight->toAirport;
                dtoVector->push_back(ticketDto);
            }

            auto balancePartDto = BalanceStatusDto::createShared();
            balancePartDto->balance = bonus->balance;
            balancePartDto->status = bonus->status;

            auto meDto = UserInfoDto::createShared();
            meDto->privilege = balancePartDto;
            meDto->tickets = dtoVector;
            auto resp = controller->createDtoResponse(Status::CODE_200, meDto);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

            std::string username_log = auth.getLogin(token);
            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/me",  
                        "GET", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        username_log, 
                        200);
            producer->PostToBroker(s);
            return _return(resp);
        }

    };

    ENDPOINT_ASYNC("POST", "/api/v1/tickets", BuyPoint) {
    ENDPOINT_ASYNC_INIT(BuyPoint)

        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&BuyPoint::buyTicketResponse);
        }
        Action buyTicketResponse( const oatpp::String& str) {
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            auto auth_token = request->getHeader("Authorization"); 
            if (!auth_token) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no auth token provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "POST", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }
            JWTAuth auth(config);
            auto token = auth.extractToken(auth_token);
            if (!auth.checkToken(token))  {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong tiken provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "POST", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto un = auth.getLogin(token);

            oatpp::Object<FullBuyRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<FullBuyRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_400, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "POST", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            un, 
                            400);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto fbrDto = FullBuyResponseDto::createShared();

            auto flightResponse = flightService->FlightGetPoint(body->flightNumber);
            if (flightResponse->getStatusCode() != 200)
            {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "POST", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            un, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(controller->getDefaultObjectMapper());


            auto ticketDto = TicketRequestDto::createShared();
            ticketDto->flightNumber = body->flightNumber;
            ticketDto->price = body->price;
            auto ticketResp = ticketService->TicketPostPoint(ticketDto, auth_token);
            if (ticketResp->getStatusCode() != 200) {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "POST", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            un, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto ticket = ticketResp->readBodyToDto<oatpp::Object<TicketResponseDto>>(
                    controller->getDefaultObjectMapper());


            auto brDto = BuyRequestDto::createShared();
            brDto->username = un;
            brDto->ticketUid = ticket->ticketUid;
            brDto->price = body->price;
            brDto->paidFromBalance = body->paidFromBalance;
            auto bonusResp = bonusService->PurchasePoint(brDto, auth_token);
            if (bonusResp->getStatusCode() != 200) {
                auto resp = controller->createResponse(Status::CODE_500);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets",  
                            "POST", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            un, 
                            500);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto bonus = bonusResp->readBodyToDto<oatpp::Object<BuyResponseDto>>(
                    controller->getDefaultObjectMapper());

            fbrDto->price = flight->price;
            fbrDto->flightNumber = flight->flightNumber;
            fbrDto->fromAirport = flight->fromAirport;
            fbrDto->toAirport = flight->toAirport;
            fbrDto->status = ticket->status;
            fbrDto->ticketUid = ticket->ticketUid;
            fbrDto->date = flight->date;
            fbrDto->paidByBonuses = bonus->paidByBonuses;
            fbrDto->paidByMoney = bonus->paidByMoney;
            auto bs = BalanceStatusDto::createShared();
            bs->status = bonus->status;
            bs->balance = bonus->balance;
            fbrDto->privilege = bs;

            auto resp = controller->createDtoResponse(Status::CODE_200, fbrDto);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/tickets",  
                        "POST", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        un, 
                        200);
            producer->PostToBroker(s);
            return _return(resp);
        }

    };

    ENDPOINT_ASYNC("DELETE", "/api/v1/tickets/{ticketUid}", ReturnPoint) {
    ENDPOINT_ASYNC_INIT(ReturnPoint)

        Action act() override {
            std::string ticket_uid = request->getPathVariable("ticketUid") ? request->getPathVariable("ticketUid") : "";
            auto startTime =  std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
           auto auth_token = request->getHeader("Authorization"); 
            if (!auth_token) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no auth token provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + ticket_uid,  
                            "DELETE", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }
            JWTAuth auth(config);
            auto token = auth.extractToken(auth_token);
            if (!auth.checkToken(token))  {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong tiken provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                std::string username_log = "unknown";
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + ticket_uid,  
                            "DELETE", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username_log, 
                            401);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto username = auth.getLogin(token);
            
            if (username.empty() || ticket_uid.empty()){
                auto dto = ErrorResponse::createShared();
                dto->message = "Invalid username or ticketUID";

                auto resp = controller->createDtoResponse(Status::CODE_400, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + ticket_uid,  
                            "DELETE", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username, 
                            400);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto ticketResp = ticketService->TicketUpdatePoint(auth_token, ticket_uid);
            if (ticketResp->getStatusCode() != 200) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found!";
                auto resp = controller->createDtoResponse(Status::CODE_404, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + ticket_uid,  
                            "DELETE", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username, 
                            404);
                producer->PostToBroker(s);
                return _return(resp);
            }

            auto bonusResp = bonusService->ReturnPoint(ticket_uid, auth_token);
            if (bonusResp->getStatusCode() != 200) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found!";
                auto resp = controller->createDtoResponse(Status::CODE_404, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                LogString s("/api/v1/tickets/" + ticket_uid,  
                            "DELETE", 
                            std::to_string(startTime), 
                            std::to_string(finishTime), 
                            std::to_string(finishTime - startTime), 
                            username, 
                            404);
                producer->PostToBroker(s);
                return _return(resp);
            }
            auto resp = controller->createResponse(Status::CODE_204);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            auto finishTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            LogString s("/api/v1/tickets/" + ticket_uid,  
                        "DELETE", 
                        std::to_string(startTime), 
                        std::to_string(finishTime), 
                        std::to_string(finishTime - startTime), 
                        username, 
                        204);
            producer->PostToBroker(s);
            return _return(resp);
        }

    };

    ENDPOINT_ASYNC("GET", "/manage/health", HealthPoint ) {
    ENDPOINT_ASYNC_INIT(HealthPoint)
        Action act() override {
            auto resp = controller->createResponse(Status::CODE_200);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

            return _return(resp);
        };
    };
};

#include OATPP_CODEGEN_END(ApiController) //<--- codegen end
