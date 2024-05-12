#include "BLFacade.h"
int BLFacade::CreateUser(const User &user) {
    return m_da_facade->CreateUser(user);
}
void BLFacade::UpdateUserByLogin( const std::string &login, const User &user) {
    m_da_facade->UpdateUserByLogin(login, user);
}
void BLFacade::DeleteUserByLogin( const std::string &login)
{
    m_da_facade->DeleteUserByLogin(login);
}
User BLFacade::GetUserByLogin( const std::string &login)
{
    return m_da_facade->GetUserByLogin(login);
}
bool BLFacade::CheckUser(const User &user)
{
    try {
        auto u = m_da_facade->GetUserByLogin(user.login);
        return u.password == user.password;
    } catch (const DatabaseException &err) { 
        return false;
    }
}
bool BLFacade::CheckClient(const Client &client)
{
    try {
        auto c = m_da_facade->GetClientByID(client.client_id);
        return c == client;
    } catch (const DatabaseException &err) { 
        return false;
    }
}

int BLFacade::CreateClient(const Client &client) {
    return m_da_facade->CreateClient(client);
}

std::string BLFacade::CreateToken(int code, const std::string &client_secret, std::string &role) {
    auto code_struct = m_da_facade->GetAuthCode(code);

    auto client = m_da_facade->GetClientByID(code_struct.client_id);
    if (client_secret != client.client_secret)
         throw DatabaseExecutionException( "wrong client" );

    auto user = m_da_facade->GetUserByLogin(code_struct.login);

    JWTAuth auth(m_config);
    role = user.role;
    return auth.createToken(user.login, user.role, code_struct.scope_string);
}
int BLFacade::CreateAuthCode(const AuthCode &code, const std::string &password) {
    auto user = m_da_facade->GetUserByLogin(code.login);
    if (password != user.password)
        throw DatabaseExecutionException( "wrong user" );

    auto client = m_da_facade->GetClientByID(code.client_id);

    return m_da_facade->CreateAuthCode(code);
}