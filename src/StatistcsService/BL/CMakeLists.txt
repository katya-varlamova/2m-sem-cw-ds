
set( PROJECT_NAME BL )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC 
    DEPENDS
        NetUI
        base
)

target_sources( ${PROJECT_NAME} 
    PRIVATE
        IDAFactory.h
        IDAFacade.h
        structs/Statistic.cpp
        structs/Statistic.h
        BLFacade.cpp
        BLFacade.h
)