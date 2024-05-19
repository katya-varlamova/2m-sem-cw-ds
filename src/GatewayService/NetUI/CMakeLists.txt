
set( PROJECT_NAME NetUI )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC 
    DEPENDS
        oatpp::oatpp
        jwt-cpp::jwt-cpp
        nlohmann_json::nlohmann_json
        base
)

target_sources( ${PROJECT_NAME} 
    PRIVATE
        OatppServer.cpp
        controller/GatewayController.cpp
        producer/KafkaProducer.cpp
)