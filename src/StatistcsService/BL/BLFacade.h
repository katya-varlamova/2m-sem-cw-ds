#pragma once

#include "IBLFacade.h"
#include <boost/di.hpp>
class BLFacade : public IBLFacade {
public:
    BOOST_DI_INJECT( BLFacade, const std::shared_ptr<IDAFactory> &factory) {
        m_da_facade = factory->CreateDAFacade();
    }
    virtual void AddStatistic( const Statistic &  statistic) override;
    virtual std::vector<Statistic> GetStatistics(std::string begin_time, std::string end_time) override;
protected:
    IDAFacadePtr m_da_facade;
};



