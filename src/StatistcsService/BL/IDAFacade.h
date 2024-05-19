#pragma once

#include <config/base_config.h>
#include <structs/Statistic.h>

class IDAFacade
{
public:
    virtual void AddStatistic( const Statistic &  statistic) = 0;
    virtual std::vector<Statistic> GetStatistics(std::string begin_time, std::string end_time) = 0;

    virtual ~IDAFacade() = default;
};

using IDAFacadePtr = std::shared_ptr<IDAFacade>;