#include "PGStatisticRepository.h"
Statistic
PGStatisticRepository::ParseStatistic( std::map<std::string, std::string> strs )
{
    return Statistic(
      strs[std::string( "username" )],
      strs[std::string( "path" )],
      std::stoi(strs[std::string( "responsecode" )]),
      strs[std::string( "method" )],
      strs[std::string( "startedat" )],
      strs[std::string( "finishedat" )],
      strs[std::string( "duration" )]
    );
}
std::vector<Statistic>
PGStatisticRepository::GetStatistics(std::string begin_time, std::string end_time)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "select username, path, responsecode, method, startedat, "
            "finishedat, duration "
            "from statistic where "
            "startedat > " + begin_time + " and "
            "finishedat < " + end_time + ";"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "GetStatistics: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such Statistics!");
    }

    std::vector<Statistic> statistics;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        std::map<std::string, std::string> res;
        for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
            char* fn = PQfname( pgRes, j );
            char* val = PQgetvalue( pgRes, i, j );
            res[std::string( fn )] = std::string( val );
        }
        statistics.push_back( ParseStatistic( res ) );
    }

    LoggerFactory::GetLogger()->LogInfo(
            "GetStatistics: no errors!"
    );
    PQclear( pgRes );
    return statistics;
}
void
PGStatisticRepository::AddStatistic( const Statistic &  statistic)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
      "insert into statistic (username, path, responsecode, method, startedat, finishedat, duration) values ('" 
      + statistic.username + "', '" 
      + statistic.path + "', " + 
      std::to_string(statistic.responsecode) + ", " + 
      "'" + statistic.method + "', " + 
      statistic.startedat  + ", " + 
      statistic.finishedat + ", " + 
      statistic.duration + ") returning id;"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "AddStatistic:" ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "create failed" );
    }

    LoggerFactory::GetLogger()->LogInfo( "AddStatistic: created" );
    PQclear( pgRes );
}
