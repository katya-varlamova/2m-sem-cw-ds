#pragma once

#include "IDAFacade.h"
#include "Repositories/IUserRepository.h"
#include "Repositories/IAuthCodeRepository.h"
#include "Repositories/IClientRepository.h"
class PGFacade : public IDAFacade
{
public:
    PGFacade(const IUserRepositoryPtr& userRepository,
    const IClientRepositoryPtr& clientRepository,
    const IAuthCodeRepositoryPtr& authCodeRepository);
    virtual User GetUserByLogin( const std::string &login ) override;
    virtual int CreateUser( const User& user ) override;
    virtual void UpdateUserByLogin( const std::string &login, const User& user ) override;
    virtual void DeleteUserByLogin( const std::string &login ) override;
    int CreateAuthCode( const AuthCode& code ) override;
    AuthCode GetAuthCode( int code ) override;
    void DeleteAuthCode( int code ) override;
    int CreateClient( const Client& client ) override;
    Client GetClientByID( int client_id ) override;

private:
    IUserRepositoryPtr m_userRepository;
    IClientRepositoryPtr m_clientRepository;
    IAuthCodeRepositoryPtr m_authCodeRepository;
};
