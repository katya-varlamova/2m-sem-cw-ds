#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class TokenRequestDto:  public oatpp::DTO {
    DTO_INIT(TokenRequestDto, DTO)
    
    DTO_FIELD(Int64, auth_code);
    DTO_FIELD(String, client_secret);
};

class TokenResponseDto:  public oatpp::DTO {
    DTO_INIT(TokenResponseDto, DTO)
    DTO_FIELD(String, access_token);
    DTO_FIELD(String, role);
};

class ClientRequestDto: public  oatpp::DTO {
    DTO_INIT(ClientRequestDto, DTO)

    DTO_FIELD(Int64, client_id);
    DTO_FIELD(String, client_secret);
};

class PersonDto: public oatpp::DTO {
    DTO_INIT(PersonDto, DTO)

    DTO_FIELD(String, firstName);
    DTO_FIELD(String, lastName);
    DTO_FIELD(String, mobilePhone);
    DTO_FIELD(String, login);
    DTO_FIELD(String, email);
    DTO_FIELD(String, password);
    DTO_FIELD(String, role);
};


#include OATPP_CODEGEN_END(DTO)