#pragma once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"


#include OATPP_CODEGEN_BEGIN(ApiClient)

class StatisticsService : public oatpp::web::client::ApiClient {
public:

  API_CLIENT_INIT(StatisticsService)

  API_CALL("GET", "/api/v1/statistics", StatisticsGetPoint, QUERY(String, begin_time), QUERY(String, end_time) )

};

#include OATPP_CODEGEN_END(ApiClient)
