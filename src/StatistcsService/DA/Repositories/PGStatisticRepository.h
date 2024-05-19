#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "IStatisticRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
class PGStatisticRepository : public IStatisticRepository
{
public:
    PGStatisticRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    virtual void AddStatistic( const Statistic &  statistic) override;
    virtual std::vector<Statistic> GetStatistics(std::string begin_time, std::string end_time) override;
    ~PGStatisticRepository() override
    {
        m_connectionCreator->FinishConnect( m_conn );
    }

protected:
    Statistic ParseStatistic( std::map<std::string, std::string> strs );

private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};
