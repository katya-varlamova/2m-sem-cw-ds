#pragma once

#include <boost/di.hpp>

#include "Connection/PGConnection.h"
#include "Facade/PGFacade.h"
#include "IDAFactory.h"
#include "Repositories/PGStatisticRepository.h"

class PGDAFactory : public IDAFactory
{
public:
    BOOST_DI_INJECT( PGDAFactory, const std::shared_ptr<BaseConfig>& config )
            : m_config( config )
    {}
    IDAFacadePtr CreateDAFacade() override;
    ~PGDAFactory() = default;

protected:
    IStatisticRepositoryPtr CreateStatisticRepository();

private:
    std::shared_ptr<BaseConfig> m_config;
    IStatisticRepositoryPtr m_StatisticRepository;
    IDAFacadePtr m_idaFacade;
};
