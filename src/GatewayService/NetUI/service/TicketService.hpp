#pragma once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "dto/TicketDtos.hpp"
#include OATPP_CODEGEN_BEGIN(ApiClient)

class TicketService : public oatpp::web::client::ApiClient {
public:

  API_CLIENT_INIT(TicketService)

  API_CALL("GET", "/api/v1/tickets", TicketsGetPoint, HEADER(String, auth_token, "Authorization"))
  API_CALL("GET", "/api/v1/tickets/{uuid}", TicketGetPoint, PATH(String, uuid), HEADER(String, auth_token, "Authorization"))
  API_CALL("POST", "/api/v1/tickets", TicketPostPoint, BODY_DTO(Object<TicketRequestDto>, ticketDto), HEADER(String, auth_token, "Authorization") )
  API_CALL("PATCH", "/api/v1/tickets/{uuid}", TicketUpdatePoint, HEADER(String, auth_token, "Authorization"), PATH(String, uuid))

};

#include OATPP_CODEGEN_END(ApiClient)