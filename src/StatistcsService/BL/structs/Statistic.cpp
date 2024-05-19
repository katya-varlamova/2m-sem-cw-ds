#include "Statistic.h"
Statistic::Statistic(const std::string& username,
               const std::string& path,
               int responsecode,
               const std::string& method,
               const std::string& startedat,
               const std::string& finishedat,
               const std::string& duration
)
        : username( username )
        , path( path )
        , responsecode( responsecode )
        , method( method )
        , startedat( startedat )
        , finishedat( finishedat )
        , duration( duration )
{
}