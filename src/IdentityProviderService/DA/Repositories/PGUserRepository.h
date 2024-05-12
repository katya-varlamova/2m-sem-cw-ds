#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "IUserRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
class PGUserRepository : public IUserRepository
{
public:
    PGUserRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    int CreateUser( const User& user ) override;
    User GetUserByLogin( const std::string &login) override;
    void UpdateUserByLogin( const std::string &login, const User& user ) override;
    void DeleteUserByLogin( const std::string &login ) override;

    ~PGUserRepository() override
    {
        m_connectionCreator->FinishConnect( m_conn );
    }

protected:
    User ParseUser( std::map<std::string, std::string> strs );

private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};
