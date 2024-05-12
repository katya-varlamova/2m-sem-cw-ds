#pragma once

#include <config/base_config.h>
#include <structs/User.h>
#include <structs/Client.h>
#include <structs/AuthCode.h>
class IDAFacade
{
public:
    virtual User GetUserByLogin( const std::string &login) = 0;
    virtual int CreateUser( const User& user ) = 0;
    virtual void UpdateUserByLogin( const std::string &login, const User& user ) = 0;
    virtual void DeleteUserByLogin( const std::string &login) = 0;
    virtual int CreateClient( const Client& client ) = 0;
    virtual Client GetClientByID( int client_id ) = 0;
    virtual int CreateAuthCode( const AuthCode& code ) = 0;
    virtual AuthCode GetAuthCode( int code ) = 0;
    virtual void DeleteAuthCode( int code ) = 0;
    virtual ~IDAFacade() = default;
};

using IDAFacadePtr = std::shared_ptr<IDAFacade>;