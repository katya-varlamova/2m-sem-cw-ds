#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)"oatpp/codegen/DTO_define.hpp"
class AuthCodeRequestDto:  public oatpp::DTO {
    DTO_INIT(AuthCodeRequestDto, DTO)
    
    DTO_FIELD(String, password);
    DTO_FIELD(String, username);
    DTO_FIELD(Int64, client_id);
    DTO_FIELD(String, scope_string);
};

class AuthCodeResponseDto:  public oatpp::DTO {
    DTO_INIT(AuthCodeResponseDto, DTO)
    DTO_FIELD(Int64, auth_code);
};

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


