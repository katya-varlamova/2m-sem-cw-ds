#include "GatewayController.hpp"

std::shared_ptr<BonusService> GatewayController::bonusService;
std::shared_ptr<FlightService> GatewayController::flightService;
std::shared_ptr<TicketService> GatewayController::ticketService;
std::shared_ptr<IdentityProviderService> GatewayController::identityProviderService;
std::shared_ptr<StatisticsService> GatewayController::statisticsService;
std::shared_ptr<BaseConfig> GatewayController::config;
std::shared_ptr<IProducer> GatewayController::producer;