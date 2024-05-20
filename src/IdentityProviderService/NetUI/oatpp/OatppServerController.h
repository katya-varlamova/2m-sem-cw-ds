#pragma once
#include "OatppDTOs.h"
#include <unistd.h>
#include <iostream>
#include <exceptions/database_exceptions.h>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/utils/String.hpp"
#include "IBLFacade.h"
#include "JWTAuth.h"
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

    ENDPOINT_ASYNC("POST", "/api/v1/authcode", AuthCodePoint ) {
    ENDPOINT_ASYNC_INIT(AuthCodePoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&AuthCodePoint::AuthCodePointReturn);
        }
        Action AuthCodePointReturn( const oatpp::String& str) {
            oatpp::Object<AuthCodeRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<AuthCodeRequestDto>>(str);
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

                return _return(resp);
            }
            AuthCode code;
            int res_code;
            code.client_id = body->client_id;
            code.scope_string = body->scope_string;
            code.login = body->username;
            try {
                res_code = facade->CreateAuthCode(code, body->password);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_500, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }
            auto dto = AuthCodeResponseDto::createShared();
            dto->auth_code = res_code;

            auto resp = controller->createDtoResponse(Status::CODE_200, dto);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

            return _return(resp);
        }
    };
    ENDPOINT_ASYNC("POST", "/api/v1/token", PointToken ) {
    ENDPOINT_ASYNC_INIT(PointToken)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&PointToken::TokenPointReturn);
        }
        Action TokenPointReturn( const oatpp::String& str) {
            oatpp::Object<TokenRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<TokenRequestDto>>(str);
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

                return _return(resp);
            }
            std::string token;
            std::string role;
            int code = body->auth_code;
            try {
                token = facade->CreateToken(body->auth_code, body->client_secret, role);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_500, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }
            auto dto = TokenResponseDto::createShared();
            dto->access_token = token;
            dto->role = role;

            auto resp = controller->createDtoResponse(Status::CODE_200, dto);
            resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");

            return _return(resp);
        }
    };

   ENDPOINT_ASYNC("POST", "/api/v1/clients", ClientPostPoint ) {

    ENDPOINT_ASYNC_INIT(ClientPostPoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&ClientPostPoint::returnResponse);
        }
        Action returnResponse( const oatpp::String& str) {
            oatpp::Object<ClientRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<ClientRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
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
            if (!auth.checkToken(token) || auth.getRole(token) != "admin")  {
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

            Client c(body->client_id, body->client_secret);
            try {
                facade->CreateClient(c);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_500, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }
            auto response = controller->createResponse(Status::CODE_200);
            return _return(response);
        }

    };

    ENDPOINT_ASYNC("POST", "/api/v1/register", PersonPostPoint ) {

    ENDPOINT_ASYNC_INIT(PersonPostPoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&PersonPostPoint::returnPersonResponse);
        }
        Action returnPersonResponse( const oatpp::String& str) {
            oatpp::Object<PersonDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<PersonDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            if (body->role == "admin") {
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
                if (!auth.checkToken(token) || auth.getRole(token) != "admin")  {
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
            }
            User u(body->firstName, body->login, body->email, body->mobilePhone, body->lastName, body->password, body->role);
            try {
                facade->CreateUser(u);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_500, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }
            auto response = controller->createResponse(Status::CODE_200);
            response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
            response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
            return _return(response);
        }

    };


    ENDPOINT_ASYNC("GET", "/api/v1/persons", PersonGetPoint ) {

    ENDPOINT_ASYNC_INIT(PersonGetPoint)

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
            auto u = facade->GetUserByLogin(username);
                auto personDto = PersonDto::createShared();
                personDto->firstName = u.name;
                personDto->email = u.email;
                personDto->login = u.login;
                personDto->mobilePhone = u.mobilePhone;
                personDto->lastName = u.lastName;
                personDto->role = u.role;
                auto response = controller->createDtoResponse(Status::CODE_200, personDto);
                response->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                response->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(response);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_404, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }
        }

    };

    ENDPOINT_ASYNC("PATCH", "/api/v1/persons", PersonUpdatePoint ) {

    ENDPOINT_ASYNC_INIT(PersonUpdatePoint)

        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&PersonUpdatePoint::returnPersonResponse);
        }
        Action returnPersonResponse( const oatpp::String& str) {
            oatpp::Object<PersonDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<PersonDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

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
            User u_old;
            try {
                u_old = facade->GetUserByLogin(username);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_404, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }

            User u(body->firstName ? *body->firstName : u_old.name,
                   body->login ? *body->login : u_old.login,
                   body->email ? *body->email : u_old.email,
                   body->mobilePhone ? *body->mobilePhone : u_old.mobilePhone,
                   body->lastName ? *body->lastName : u_old.lastName,
                   body->password ? *body->password : u_old.password,
                   body->role ? *body->role : u_old.role); 
            try {
                facade->UpdateUserByLogin(username, u);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_500, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }
            auto personDto = PersonDto::createShared();
            personDto->firstName = u.name;
            personDto->email = u.email;
            personDto->login = u.login;
            personDto->mobilePhone = u.mobilePhone;
            personDto->lastName = u.lastName;
            personDto->role = u.role;
            return _return(controller->createDtoResponse(Status::CODE_200, personDto));
        }

    };
    ENDPOINT_ASYNC("DELETE", "/api/v1/persons", PersonDeletePoint ) {
    ENDPOINT_ASYNC_INIT(PersonDeletePoint)

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
                facade->GetUserByLogin(username);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found with such ID!";
                auto resp = controller->createDtoResponse(Status::CODE_404, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }

            try {
                facade->DeleteUserByLogin(username);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                auto resp = controller->createDtoResponse(Status::CODE_500, dto);
                resp->putHeaderIfNotExists("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS, DELETE");
                resp->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
                resp->putHeaderIfNotExists("Access-Control-Max-Age", "1728000");
                return _return(resp);
            }

            return _return(controller->createResponse(Status::CODE_204));
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

