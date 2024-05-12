#include "PGClientRepository.h"
Client
PGClientRepository::ParseClient( std::map<std::string, std::string> strs )
{
    return Client(
      std::stoi( strs[std::string( "client_id" )] ),
      strs[std::string( "client_secret" )]
    );
}
Client
PGClientRepository::GetClientByID( int id )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "select * from client where client_id = " + std::to_string( id ) + ";"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK || PQntuples( pgRes ) == 0 ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "GetClientByID: no such Client: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "no such Client" );
    }

    std::map<std::string, std::string> res;
    for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
        char* fn = PQfname( pgRes, j );
        char* name = PQgetvalue( pgRes, 0, j );
        res[std::string( fn )] = std::string( name );
    }
    auto Client = ParseClient( res );

    LoggerFactory::GetLogger()->LogInfo( "GetClientByID: Client found" );
    PQclear( pgRes );
    return Client;
}
int
PGClientRepository::CreateClient( const Client& Client )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
            "insert into client (client_secret) values ('" 
            + Client.client_secret + "') returning client_id;"
    );

    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string( "CreateClient: registration failed: " ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "registration failed" );
    }
    int id;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        int fn = PQfnumber( pgRes, "client_id" );
        char* name = PQgetvalue( pgRes, i, fn );
        id = std::strtol( name, &name, 10 );
    }

    LoggerFactory::GetLogger()->LogInfo(
            ( std::string( "CreateClient: registration succeed with uid = " ) +
              std::to_string( id ) )
                    .c_str()
    );

    PQclear( pgRes );
    return id;
}
