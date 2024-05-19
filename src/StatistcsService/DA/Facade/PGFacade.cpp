#include "PGFacade.h"
PGFacade::PGFacade(
  const IStatisticRepositoryPtr& StatisticRepository
) : m_StatisticRepository( StatisticRepository )
{
}
void PGFacade::AddStatistic( const Statistic &  statistic) {
    m_StatisticRepository->AddStatistic(statistic);
}
std::vector<Statistic> PGFacade::GetStatistics(std::string begin_time, std::string end_time) {
    return m_StatisticRepository->GetStatistics(begin_time, end_time);
}
