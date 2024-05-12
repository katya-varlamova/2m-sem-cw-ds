#pragma once

#include "IBLFacade.h"
#include <boost/di.hpp>
#include "JWTAuth.h"
class BLFacade : public IBLFacade {
public:
    BOOST_DI_INJECT( BLFacade, const std::shared_ptr<IDAFactory> &factory, 
    const std::shared_ptr<BaseConfig>& config )
            : m_config( config ) {
        m_da_facade = factory->CreateDAFacade();
    }
    User GetUserByLogin( const std::string &login) override;
    void UpdateUserByLogin( const std::string &login, const User &user) override;
    void DeleteUserByLogin( const std::string &login) override;
    int CreateUser(const User &user) override;

    int CreateClient(const Client &client) override;
    bool CheckClient(const Client &client) override;
    bool CheckUser(const User &user) override;

    std::string CreateToken(int code, const std::string &client_secret, std::string &role) override;
    int CreateAuthCode(const AuthCode  &code, const std::string &password) override;

protected:
    IDAFacadePtr m_da_facade;
    std::shared_ptr<BaseConfig> m_config;
};



