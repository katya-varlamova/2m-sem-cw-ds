#include "PGFacade.h"
PGFacade::PGFacade(
  const IUserRepositoryPtr& userRepository,
  const IClientRepositoryPtr& clientRepository,
  const IAuthCodeRepositoryPtr& authCodeRepository
) : m_userRepository( userRepository ),  
m_clientRepository (clientRepository), 
m_authCodeRepository (authCodeRepository)
{
}

User
PGFacade::GetUserByLogin( const std::string &login )
{
    return m_userRepository->GetUserByLogin( login );
}

int
PGFacade::CreateUser( const User& user )
{
    return m_userRepository->CreateUser( user );
}
void
PGFacade::UpdateUserByLogin( const std::string &login, const User& user )
{
    m_userRepository->UpdateUserByLogin(login, user );
}
void
PGFacade::DeleteUserByLogin( const std::string &login )
{
    m_userRepository->DeleteUserByLogin(login);
}
#include "logger/LoggerFactory.h"
int PGFacade::CreateAuthCode( const AuthCode& code ) {
    return m_authCodeRepository->CreateAuthCode(code);
}
AuthCode PGFacade::GetAuthCode( int code ) {
    return m_authCodeRepository->GetAuthCode(code);
}
void PGFacade::DeleteAuthCode( int code ) {
    m_authCodeRepository->DeleteAuthCode(code);
}
int PGFacade::CreateClient( const Client& client ) {
    return m_clientRepository->CreateClient(client);
}
Client PGFacade::GetClientByID( int client_id ) {
    return m_clientRepository->GetClientByID(client_id);
}