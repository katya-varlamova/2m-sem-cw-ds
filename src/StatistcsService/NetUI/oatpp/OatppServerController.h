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
#include "logger/LoggerFactory.h"
#include OATPP_CODEGEN_BEGIN(ApiController)"oatpp/codegen/ApiController_define.hpp"


class OatppServerController : public oatpp::web::server::api::ApiController {

protected:
    OatppServerController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
    {}
public:
    static IBLFacadePtr facade;
    static std::shared_ptr<OatppServerController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                      objectMapper)){
        return std::shared_ptr<OatppServerController>(new OatppServerController(objectMapper));
    }

    ENDPOINT_ASYNC("GET", "/api/v1/statistics", StatisticsGetPoint ) {

    ENDPOINT_ASYNC_INIT(StatisticsGetPoint)
        Action act() override {
            std::string begin_time = request->getQueryParameter("begin_time");
            std::string end_time = request->getQueryParameter("end_time");
            try {
                auto statistics = facade->GetStatistics(begin_time, end_time);
                auto statisticsDto = StatisticsResponseDto::createShared();

                oatpp::Vector<oatpp::Object<StatisticRequestDto>> dtoVector({});
                for (const auto & statistic : statistics) {
                    auto statisticDto = StatisticRequestDto::createShared();
                    statisticDto->userName = statistic.username ;
                    statisticDto->path = statistic.path;
                    statisticDto->responseCode = statistic.responsecode;
                    statisticDto->method = statistic.method;
                    statisticDto->startedAt = statistic.startedat;
                    statisticDto->finishedAt = statistic.finishedat;
                    statisticDto->duration = statistic.duration ;

                    dtoVector->push_back(statisticDto);
                }
                statisticsDto->items = dtoVector;
                return _return(controller->createDtoResponse(Status::CODE_200, statisticsDto));
            } catch (const DatabaseException &err) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };
    ENDPOINT_ASYNC("POST", "/api/v1/statistics", StatisticPostPoint ) {

    ENDPOINT_ASYNC_INIT(StatisticPostPoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&StatisticPostPoint::returnResponse);
        }
        Action returnResponse( const oatpp::String& str) {
            oatpp::Object<StatisticRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<StatisticRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            try {
                Statistic statistic;
                statistic.username =  body->userName;
                statistic.path =  body->path;
                statistic.responsecode =  body->responseCode;
                statistic.method =  body->method;
                statistic.startedat =  body->startedAt;
                statistic.finishedat =  body->finishedAt;
                statistic.duration =  body->duration;
                facade->AddStatistic(statistic);
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

