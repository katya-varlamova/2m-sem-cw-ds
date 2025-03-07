#pragma once
#include "oatpp/network/Server.hpp"
#include "OatppServerController.h"
#include "OatppServerSettings.h"
#include "IServer.h"
#include <boost/di.hpp>
class OatppServer : public IServer {
public:
    BOOST_DI_INJECT(OatppServer, const IBLFacadePtr &facade, const std::shared_ptr<BaseConfig>& config ) {
        OatppServerController::config = config;
        OatppServerController::facade = facade;
    }
    void run() override;
};



