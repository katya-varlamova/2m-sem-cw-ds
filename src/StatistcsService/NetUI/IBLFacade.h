#pragma once

#include <config/base_config.h>
#include "structs/Statistic.h"
#include "IDAFactory.h"
class IBLFacade{
public:
    virtual void AddStatistic( const Statistic &  statistic) = 0;
    virtual std::vector<Statistic> GetStatistics(std::string begin_time, std::string end_time) = 0;
    virtual ~IBLFacade() = default;
};

using IBLFacadePtr = std::shared_ptr<IBLFacade>;


