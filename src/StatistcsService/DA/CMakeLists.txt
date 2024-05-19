set (NEED_TESTS TRUE)

set( PROJECT_NAME DA )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC
        DEPENDS
        Boost::boost
        libpqxx::pqxx
        base
        BL
        )

target_sources( ${PROJECT_NAME}
        PRIVATE
        Connection/PGConnection.cpp
        Facade/PGFacade.cpp
        Factory/PGFactory.cpp
        Repositories/PGStatisticRepository.cpp

        )