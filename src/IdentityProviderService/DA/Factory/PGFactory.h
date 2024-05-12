#pragma once

#include <boost/di.hpp>

#include "Connection/PGConnection.h"
#include "Facade/PGFacade.h"
#include "IDAFactory.h"
#include "Repositories/PGUserRepository.h"
#include "Repositories/PGAuthCodeRepository.h"
#include "Repositories/PGClientRepository.h"
class PGDAFactory : public IDAFactory
{
public:
    BOOST_DI_INJECT( PGDAFactory, const std::shared_ptr<BaseConfig>& config )
            : m_config( config )
    {}
    IDAFacadePtr CreateDAFacade() override;
    ~PGDAFactory() = default;

protected:
    IUserRepositoryPtr CreateUserRepository();
    IAuthCodeRepositoryPtr CreateAuthCodeRepository();
    IClientRepositoryPtr CreateClientRepository();

private:
    std::shared_ptr<BaseConfig> m_config;
    IUserRepositoryPtr m_userRepository;
    IClientRepositoryPtr m_clientRepository;
    IAuthCodeRepositoryPtr m_authCodeRepository;
    IDAFacadePtr m_idaFacade;
};
