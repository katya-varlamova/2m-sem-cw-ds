#include "PGUserRepository.h"
User
PGUserRepository::ParseUser( std::map<std::string, std::string> strs )
{
    return User(
      strs[std::string( "name" )],
      strs[std::string( "login" )],
      strs[std::string( "email" )],
      strs[std::string( "mobilePhone" )],
      strs[std::string( "lastName" )],
      strs[std::string( "password" )],
      strs[std::string( "role" )]
    );
}
User
PGUserRepository::GetUserByLogin( const std::string &login)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "select * from person where login = '" + login + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK || PQntuples( pgRes ) == 0 ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "GetUserByLgin: no such user: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "no such user" );
    }

    std::map<std::string, std::string> res;
    for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
        char* fn = PQfname( pgRes, j );
        char* name = PQgetvalue( pgRes, 0, j );
        res[std::string( fn )] = std::string( name );
    }
    auto user = ParseUser( res );

    LoggerFactory::GetLogger()->LogInfo( "GetUserByLogin: user found" );
    PQclear( pgRes );
    return user;
}
int
PGUserRepository::CreateUser( const User& user )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
            "insert into person (login, name, mobilePhone,  email, lastName, password, role) values ('" 
            + user.login + "', '"
            + user.name + "', '"
            + user.mobilePhone + "', '"
            + user.email + "', '"
            + user.lastName + "', '"
            + user.password + "', '"
            + user.role + "') returning login;"
    );
    
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string( "CreateUser: registration failed: " ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "registration failed" );
    }

    PQclear( pgRes );
    return 0;
}

void
PGUserRepository::UpdateUserByLogin( const std::string &login, const User& user )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "update Person set "
            "name = '" + user.name + "', "
            "mobilePhone = '" + user.mobilePhone + "', "
            "login = '" + user.login + "', "
            "password = '" + user.password + "', "
            "role = '" + user.role + "', "
            "lastName = '" + user.lastName + "', "
            "email = '"+ user.email + "' where login = '" +
            login + "';"
    );

    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "UpdateUserByID: user update failed: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "execution error" );
    }
    LoggerFactory::GetLogger()->LogInfo( "UpdateUserByID: user was updated" );
    PQclear( pgRes );
}
void
PGUserRepository::DeleteUserByLogin( const std::string &login )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "delete from person where login = '" + login + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "DeleteUserByID: there was no such user: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "there was no such user" );
    }

    LoggerFactory::GetLogger()->LogInfo( "DeleteUserByID: user was deleted" );
    PQclear( pgRes );
}
