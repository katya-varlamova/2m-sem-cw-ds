#pragma once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "dto/IdentityProviderDtos.hpp"
#include OATPP_CODEGEN_BEGIN(ApiClient)

class IdentityProviderService : public oatpp::web::client::ApiClient {
public:

  API_CLIENT_INIT(IdentityProviderService)

  API_CALL("POST", "/api/v1/token", TokenGetPoint, BODY_DTO(Object<TokenRequestDto>, request))
  API_CALL("POST", "/api/v1/clients", ClientPostPoint, BODY_DTO(Object<ClientRequestDto>, request), HEADER(String, auth_token, "Authorization"))
  API_CALL("GET", "/api/v1/persons", PersonGetPoint, HEADER(String, auth_token, "Authorization") )
};

#include OATPP_CODEGEN_END(ApiClient)