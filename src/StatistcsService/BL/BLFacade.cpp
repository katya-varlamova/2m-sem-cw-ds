#include "BLFacade.h"

void BLFacade::AddStatistic( const Statistic &  statistic) {
    return m_da_facade->AddStatistic(statistic);
}
std::vector<Statistic> BLFacade::GetStatistics(std::string begin_time, std::string end_time) {
    return m_da_facade->GetStatistics(begin_time, end_time);
}
