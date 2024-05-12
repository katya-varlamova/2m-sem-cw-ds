#pragma once

#include <memory>
#include <vector>
#include "structs/AuthCode.h"

class IAuthCodeRepository
{
public:
    virtual int CreateAuthCode( const AuthCode& code ) = 0;
    virtual AuthCode GetAuthCode( int code ) = 0;
    virtual void DeleteAuthCode( int code ) = 0;
    virtual ~IAuthCodeRepository() = default;
};
using IAuthCodeRepositoryPtr = std::shared_ptr<IAuthCodeRepository>;