#pragma once
#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

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
class AuthResponseDto:  public oatpp::DTO {
    DTO_INIT(AuthResponseDto, DTO)
    DTO_FIELD(Int64, client_id);
    DTO_FIELD(String, scope_string);
    DTO_FIELD(String, auth_url);
};

class CallbackRequestDto:  public oatpp::DTO {
    DTO_INIT(CallbackRequestDto, DTO)
    DTO_FIELD(Int64, auth_code);
};

class FullTicketResponseDto: public oatpp::DTO {
    DTO_INIT(FullTicketResponseDto, DTO)
    DTO_FIELD(String, ticketUid);
    DTO_FIELD(Int64, price);
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(String, status);
    DTO_FIELD(String, fromAirport);
    DTO_FIELD(String, toAirport);
    DTO_FIELD(String, date);
};
class FullTicketsResponseDto: public oatpp::DTO {
    DTO_INIT(FullTicketsResponseDto, DTO)
    DTO_FIELD(Vector<Object<FullTicketResponseDto>>, tickets);
};
class BalanceStatusDto: public oatpp::DTO {
    DTO_INIT(BalanceStatusDto, DTO)
    DTO_FIELD(Int64, balance);
    DTO_FIELD(String, status);
};
class UserInfoDto: public oatpp::DTO {
    DTO_INIT(UserInfoDto, DTO)
    DTO_FIELD(Vector<Object<FullTicketResponseDto>>, tickets);
    DTO_FIELD(Object<BalanceStatusDto>, privilege);

};
class FullBuyRequestDto: public oatpp::DTO {
    DTO_INIT(FullBuyRequestDto, DTO)
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(Int64, price);
    DTO_FIELD(Boolean, paidFromBalance);
};

class FullBuyResponseDto: public oatpp::DTO {
    DTO_INIT(FullBuyResponseDto, DTO)
    DTO_FIELD(String, ticketUid);
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(String, fromAirport);
    DTO_FIELD(String, toAirport);
    DTO_FIELD(String, date);
    DTO_FIELD(Int64, price);
    DTO_FIELD(String, status);
    DTO_FIELD(Int64, paidByMoney);
    DTO_FIELD(Int64, paidByBonuses);
    DTO_FIELD(Object<BalanceStatusDto>, privilege);
};

#include OATPP_CODEGEN_END(DTO)