#pragma once

#include <config/base_config.h>
#include "structs/User.h"
#include "structs/AuthCode.h"
#include "structs/Client.h"
#include "IDAFactory.h"
class IBLFacade{
public:
    virtual User GetUserByLogin( const std::string &login ) = 0;
    virtual int CreateUser( const User& user ) = 0;
    virtual void UpdateUserByLogin( const std::string &login, const User& user ) = 0;
    virtual void DeleteUserByLogin( const std::string &login ) = 0;

    virtual int CreateClient(const Client &client) = 0;
    virtual bool CheckClient(const Client &client) = 0;
    virtual bool CheckUser(const User &user) = 0;

    virtual std::string CreateToken(int code, const std::string &client_secret, std::string &role) = 0;
    virtual int CreateAuthCode(const AuthCode  &code, const std::string &password) = 0;

    virtual ~IBLFacade() = default;
};

using IBLFacadePtr = std::shared_ptr<IBLFacade>;


