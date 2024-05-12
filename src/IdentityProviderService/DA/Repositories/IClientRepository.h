#pragma once

#include <memory>
#include <vector>
#include "structs/Client.h"

class IClientRepository
{
public:
    virtual int CreateClient( const Client& client ) = 0;
    virtual Client GetClientByID( int client_id ) = 0;
    virtual ~IClientRepository() = default;
};
using IClientRepositoryPtr = std::shared_ptr<IClientRepository>;