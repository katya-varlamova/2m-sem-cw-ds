#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "AppComponent.hpp"

#include "controller/GatewayController.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"
#include <boost/di.hpp>
#include <memory>
#include "config/base_config.h"
#include "config/base_sections.h"
#include "exceptions/database_exceptions.h"

class IServer {
public:
    virtual void run() = 0;
};
using IServerPtr = std::shared_ptr<IServer>;

class OatppServer : public IServer {
public:
    BOOST_DI_INJECT(OatppServer, const std::shared_ptr<BaseConfig>& config ) {
        GatewayController::config = config;
    }
  void run() override;
};
