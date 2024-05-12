#include "PGFactory.h"
IDAFacadePtr
PGDAFactory::CreateDAFacade()
{
    if ( m_idaFacade )
        return m_idaFacade;

    return (
      m_idaFacade = IDAFacadePtr( new PGFacade(
        CreateUserRepository(),
        CreateClientRepository(),
        CreateAuthCodeRepository()
      ) )
    );
}
IUserRepositoryPtr
PGDAFactory::CreateUserRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_userRepository ? m_userRepository
                            : ( m_userRepository = IUserRepositoryPtr(
                                  new PGUserRepository( connectionCreator )
                                ) );
}
IAuthCodeRepositoryPtr
PGDAFactory::CreateAuthCodeRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_authCodeRepository ? m_authCodeRepository
                            : ( m_authCodeRepository = IAuthCodeRepositoryPtr(
                                  new PGAuthCodeRepository( connectionCreator )
                                ) );
}

IClientRepositoryPtr
PGDAFactory::CreateClientRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_clientRepository ? m_clientRepository
                            : ( m_clientRepository = IClientRepositoryPtr(
                                  new PGClientRepository( connectionCreator )
                                ) );
}
