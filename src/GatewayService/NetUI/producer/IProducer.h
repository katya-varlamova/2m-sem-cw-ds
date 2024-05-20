#pragma once
#include <iostream>
#include <string>

struct LogString {
    LogString(const std::string & path, 
    std::string method, 
    std::string startedAt, 
    std::string finishedAt, 
    std::string duration,
    std::string userName,
    int responseCode) {
        this->path = path;
        this->method = method;
        this->userName = userName;
        this->responseCode = responseCode;
        this->startedAt = startedAt;
        this->finishedAt = finishedAt;
        this->duration = duration;
    }
    std::string path, method, userName;
    int responseCode;
    std::string startedAt, finishedAt, duration;
    int y = 0;
};

class IProducer {
public:
    virtual void PostToBroker(const LogString& logData) = 0;
    virtual ~IProducer() = default;
};