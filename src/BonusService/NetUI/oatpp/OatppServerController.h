#pragma once
#include "OatppDTOs.h"
#include <unistd.h>
#include <iostream>
#include <exceptions/database_exceptions.h>
#include <string>
#include <cstring>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/utils/String.hpp"
#include "IBLFacade.h"
#include "oatpp/JWTAuth.h"
#include OATPP_CODEGEN_BEGIN(ApiController)"oatpp/codegen/ApiController_define.hpp"


class OatppServerController : public oatpp::web::server::api::ApiController {

protected:
    OatppServerController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
    {}
public:
    static IBLFacadePtr facade;
    static std::shared_ptr<BaseConfig> config;
    static std::shared_ptr<OatppServerController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                      objectMapper)){
        return std::shared_ptr<OatppServerController>(new OatppServerController(objectMapper));
    }

    ENDPOINT_ASYNC("GET", "/api/v1/balance", BalanceGetPoint ) {

    ENDPOINT_ASYNC_INIT(BalanceGetPoint)

        Action act() override {
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

                return _return(resp);
            }

            auto username = auth.getLogin(token);

            try {

                auto balance = facade->GetBalanceByUsername(username);
                auto balanceResponseDto = BalanceResponseDto::createShared();

                balanceResponseDto->balance = balance.balance;
                balanceResponseDto->status = balance.status;
                oatpp::Vector<Object<HistoryDto>> dtoVector({});
                for (const auto & record : balance.history) {
                    auto h = HistoryDto::createShared();
                    h->balanceDiff = record.balanceDiff;
                    h->date = record.date;
                    h->operationType = record.operationType;
                    h->ticketUid = record.ticketUid;
                    dtoVector->push_back(h);
                }
                balanceResponseDto->history = dtoVector;
                return _return(controller->createDtoResponse(Status::CODE_200, balanceResponseDto));
            } catch (const DatabaseException &err) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };
    ENDPOINT_ASYNC("PATCH", "/api/v1/purchase", PurchasePoint ) {

    ENDPOINT_ASYNC_INIT(PurchasePoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&PurchasePoint::PurchasePointResponse);
        }
        Action PurchasePointResponse( const oatpp::String& str) {
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

                return _return(resp);
            }
            JWTAuth auth(config);
            auto token = auth.extractToken(auth_token);
            if (!auth.checkToken(token))  {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong token provided");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                return _return(resp);
            }

            auto un = auth.getLogin(token);

            oatpp::Object<BuyRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<BuyRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }


            if (un != std::string(body->username))
            {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid auth for such request");
                dto->errors = errors;
                auto resp = controller->createDtoResponse(Status::CODE_401, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

                return _return(resp);
            }

            try {
                BuyRequest req;
                req.username = body->username;
                req.ticket_uid = body->ticketUid;
                req.price = body->price;
                req.paidFromBalance = body->paidFromBalance;

                auto resp = facade->PerfromBuyOperation(req);
                auto respDto = BuyResponseDto::createShared();

                respDto->balance = resp.balance;
                respDto->status = resp.status;
                respDto->paidByBonuses = resp.paidByBonuses;
                respDto->paidByMoney = resp.paidByMoney;

                auto response = controller->createDtoResponse(Status::CODE_200, respDto);

                return _return(response);
            } catch (const DatabaseException &err) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };

    ENDPOINT_ASYNC("PATCH", "/api/v1/return", ReturnPoint ) {

    ENDPOINT_ASYNC_INIT(ReturnPoint)
        Action act() override {
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

                return _return(resp);
            }

            auto username = auth.getLogin(token);

            std::string ticket_uid = request->getHeader("ticket_uid") ? request->getHeader("ticket_uid") : "";
            if (username.empty() || ticket_uid.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no user info provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            try {
                facade->PerfromReturnOperation(username, ticket_uid);
                auto response = controller->createResponse(Status::CODE_200);

                return _return(response);
            } catch (const DatabaseException &err) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };

    ENDPOINT_ASYNC("GET", "/manage/health", HealthPoint ) {
        ENDPOINT_ASYNC_INIT(HealthPoint)
        Action act() override {
            return _return(controller->createResponse(Status::CODE_200));
        };
    };

};

#include OATPP_CODEGEN_BEGIN(ApiController)"oatpp/codegen/ApiController_define.hpp"

using OatppServerControllerPtr = std::shared_ptr<OatppServerController>;

