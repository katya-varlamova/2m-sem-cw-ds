
#include "OatppServer.hpp"

void OatppServer::run() {
    oatpp::base::Environment::init();
    AppComponent components(
            {"0.0.0.0", 8080},
            {"flight-service", 8060},
            {"ticket-service", 8070},
            {"bonus-service", 8050},
            {"identity-provider-service", 8090},
            {"statistics-service", 8020}
    );

    GatewayController::bonusService = components.bonusService.getObject();
    GatewayController::flightService = components.flightService.getObject();
    GatewayController::ticketService = components.ticketService.getObject();
    GatewayController::identityProviderService = components.identityProviderService.getObject();
    GatewayController::statisticsService = components.statisticsService.getObject();

    auto router = components.httpRouter.getObject();

    router->addController(GatewayController::createShared());


    oatpp::network::Server server(
            components.serverConnectionProvider.getObject(),
            components.serverConnectionHandler.getObject());

    server.run();
    oatpp::base::Environment::destroy();

}

