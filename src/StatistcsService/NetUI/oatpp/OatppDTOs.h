#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)"oatpp/codegen/DTO_define.hpp"

class StatisticRequestDto: public oatpp::DTO {
    DTO_INIT(StatisticRequestDto, DTO)
    DTO_FIELD(String, path);
    DTO_FIELD(Int64, responseCode);
    DTO_FIELD(String, method);
    DTO_FIELD(String, startedAt);
    DTO_FIELD(String, finishedAt);
    DTO_FIELD(String, duration);
    DTO_FIELD(String, userName);
};
class StatisticsResponseDto: public oatpp::DTO {
    DTO_INIT(StatisticsResponseDto, DTO)
    DTO_FIELD(Vector<Object<StatisticRequestDto>>, items);
};
class ValidationErrorResponse: public oatpp::DTO {
    DTO_INIT(ValidationErrorResponse, DTO)

    DTO_FIELD(String, message);
    DTO_FIELD(Vector<String>, errors);
};
class ErrorResponse: public oatpp::DTO {
    DTO_INIT(ErrorResponse, DTO)

    DTO_FIELD(String, message);
};
#include OATPP_CODEGEN_END(DTO)"oatpp/codegen/DTO_undef.hpp"


