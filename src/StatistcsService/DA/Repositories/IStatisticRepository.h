#pragma once

#include <memory>
#include <vector>
#include "structs/Statistic.h"

class IStatisticRepository
{
public:
    virtual void AddStatistic( const Statistic &  statistic) = 0;
    virtual std::vector<Statistic> GetStatistics(std::string begin_time, std::string end_time) = 0;
    virtual ~IStatisticRepository() = default;
};
using IStatisticRepositoryPtr = std::shared_ptr<IStatisticRepository>;