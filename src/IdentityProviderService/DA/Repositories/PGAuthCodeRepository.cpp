#include "PGAuthCodeRepository.h"
AuthCode
PGAuthCodeRepository::ParseAuthCode( std::map<std::string, std::string> strs )
{
    return AuthCode(
      std::stoi( strs[std::string( "code" )] ),
      std::stoi( strs[std::string( "client_id" )]),
      strs[std::string( "login" )],
      strs[std::string( "scope_string" )]
    );
}

AuthCode
PGAuthCodeRepository::GetAuthCode( int code )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "select * from AuthCode where code = " + std::to_string( code ) + ";"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK || PQntuples( pgRes ) == 0 ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "GetAuthCodeByID: no such AuthCode: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "no such AuthCode" );
    }

    std::map<std::string, std::string> res;
    for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
        char* fn = PQfname( pgRes, j );
        char* name = PQgetvalue( pgRes, 0, j );
        res[std::string( fn )] = std::string( name );
    }
    auto AuthCode = ParseAuthCode( res );

    LoggerFactory::GetLogger()->LogInfo( "GetAuthCode: AuthCode found" );
    PQclear( pgRes );
    return AuthCode;
}
int
PGAuthCodeRepository::CreateAuthCode( const AuthCode& code )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "insert into AuthCode (client_id, login, scope_string) values (" 
            + std::to_string( code.client_id ) + ", '"
            + code.login + "', '"
            + code.scope_string + "') returning code;"
    );

    PGresult* pgRes = PQexec( m_conn, query.c_str() );
    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string( "CreateAuthCode: failed: " ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "create code failed" );
    }
    int id;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        int fn = PQfnumber( pgRes, "code" );
        char* name = PQgetvalue( pgRes, i, fn );
        id = std::strtol( name, &name, 10 );
    }

    LoggerFactory::GetLogger()->LogInfo(
            ( std::string( "CreateAuthCode: succeed with code = " ) +
              std::to_string( id ) )
                    .c_str()
    );

    PQclear( pgRes );
    return id;
}

void
PGAuthCodeRepository::DeleteAuthCode( int id )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "delete from AuthCode where code = " + std::to_string( id ) + ";"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "DeleteAuthCodeByID: there was no such AuthCode: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "there was no such AuthCode" );
    }

    LoggerFactory::GetLogger()->LogInfo( "DeleteAuthCodeByID: AuthCode was deleted" );
    PQclear( pgRes );
}
