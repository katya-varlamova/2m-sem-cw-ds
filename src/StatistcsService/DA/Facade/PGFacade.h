#pragma once

#include "IDAFacade.h"
#include "Repositories/IStatisticRepository.h"
class PGFacade : public IDAFacade
{
public:
    PGFacade(const IStatisticRepositoryPtr& StatisticRepository);
    virtual void AddStatistic( const Statistic &  statistic) override;
    virtual std::vector<Statistic> GetStatistics(std::string begin_time, std::string end_time) override;

private:
    IStatisticRepositoryPtr m_StatisticRepository;
};
