#pragma once

#include <string>

struct AuthCode
{
    AuthCode() = default;
    AuthCode(
      int code, 
      int client_id,  
      const std::string& login,
      const std::string& scope_string) 
      : code (code), client_id( client_id ), login (login),  scope_string (scope_string)
      { }
    int code, client_id;
    std::string login, scope_string;
    bool operator==( const AuthCode& l ) const
    {
        return l.code == this->code && 
        l.client_id == this->client_id && 
        l.login == this->login &&
        l.scope_string == this->scope_string;
    }
};