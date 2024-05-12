#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "IAuthCodeRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
class PGAuthCodeRepository : public IAuthCodeRepository
{
public:
    PGAuthCodeRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    int CreateAuthCode( const AuthCode& code ) override;
    AuthCode GetAuthCode( int code ) override;
    void DeleteAuthCode( int code ) override;

    ~PGAuthCodeRepository() override
    {
        m_connectionCreator->FinishConnect( m_conn );
    }

protected:
    AuthCode ParseAuthCode( std::map<std::string, std::string> strs );

private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};