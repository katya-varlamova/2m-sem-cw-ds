#pragma once

#include <string>

class Statistic {
public:
    Statistic() = default;
    Statistic(const std::string& username,
            const std::string& path,
            int responsecode,
            const std::string& method,
            const std::string& startedat,
            const std::string& finishedat,
            const std::string& duration
    );

    std::string username, path;
    int responsecode;
    std::string method, startedat, finishedat, duration;
    bool operator==( const Statistic& l ) const
    {
        return l.path == this->path && l.method == this->method &&
               l.username == this->username && l.responsecode == this->responsecode &&
               l.startedat == this->startedat && l.finishedat == this->finishedat &&  l.duration == this->duration;
    }
};



