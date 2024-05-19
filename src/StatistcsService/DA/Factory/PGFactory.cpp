#include "PGFactory.h"
IDAFacadePtr
PGDAFactory::CreateDAFacade()
{
    if ( m_idaFacade )
        return m_idaFacade;

    return (
      m_idaFacade = IDAFacadePtr( new PGFacade(
        CreateStatisticRepository()
      ) )
    );
}
IStatisticRepositoryPtr
PGDAFactory::CreateStatisticRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_StatisticRepository ? m_StatisticRepository
                            : ( m_StatisticRepository = IStatisticRepositoryPtr(
                                  new PGStatisticRepository( connectionCreator )
                                ) );
}
