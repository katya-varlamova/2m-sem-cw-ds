#pragma once

#include <memory>
#include <vector>
#include "structs/User.h"

class IUserRepository
{
public:
    virtual int CreateUser( const User& session ) = 0;
    virtual User GetUserByLogin( const std::string &login) = 0;
    virtual void UpdateUserByLogin( const std::string &login, const User& user ) = 0;
    virtual void DeleteUserByLogin( const std::string &login ) = 0;
    virtual ~IUserRepository() = default;
};
using IUserRepositoryPtr = std::shared_ptr<IUserRepository>;