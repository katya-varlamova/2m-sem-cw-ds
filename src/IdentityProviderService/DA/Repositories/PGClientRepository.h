#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "IClientRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"

class PGClientRepository : public IClientRepository
{
public:
    PGClientRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    int CreateClient( const Client& client ) override;
    Client GetClientByID( int client_id ) override;
    ~PGClientRepository() override
    {
        m_connectionCreator->FinishConnect( m_conn );
    }

protected:
    Client ParseClient( std::map<std::string, std::string> strs );

private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};