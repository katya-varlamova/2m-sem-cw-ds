#pragma once
#include <iostream>
#include <jwt-cpp/jwt.h>
#include "logger/LoggerFactory.h"
#include "config/base_config.h"
#include "config/base_sections.h"
#include "exceptions/database_exceptions.h"
#include <cstdlib>
class JWTAuth
{
private:
    std::string m_publicKey;
public:
    JWTAuth(const std::shared_ptr<BaseConfig>& conf) {
        m_publicKey = conf->GetStringField( { JwksSection });
    
        for (size_t i = 0; i < m_publicKey.size(); i++)
        {
            if (m_publicKey[i] == '\'')
                m_publicKey[i] = '"';
        }
    };
    bool checkToken(const std::string &token)
    {
        auto decoded_jwt = jwt::decode(token);
        auto jwks = jwt::parse_jwks(m_publicKey.c_str());
        auto jwk = jwks.get_jwk(decoded_jwt.get_key_id());
        auto issuer = decoded_jwt.get_issuer();

        if (!issuer.empty()) {
            const auto modulus = jwk.get_jwk_claim("n").as_string();
		    const auto exponent = jwk.get_jwk_claim("e").as_string();

            auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::rs256(jwt::helper::create_public_key_from_rsa_components(modulus, exponent)))
                                .with_issuer(issuer)
                                .leeway(60UL); 

		    try {
                verifier.verify(decoded_jwt);
            }
            catch (...) {
                return false;
            }
            return true;
        }
        return false;
    }
    std::string getLogin(const std::string &token)
    {
        auto decoded = jwt::decode(token);
        return decoded.get_issuer();
    }
    std::string getRole(const std::string &token)
    {
        auto decoded = jwt::decode(token);
        return  decoded.get_payload_claim("role").as_string();
    }
    std::string extractToken(const std::string& input) {
        size_t pos = input.find("Bearer");
        if (pos != std::string::npos) {
            std::string token = input.substr(pos + 7); 
            return token;
        } else {
            return "";
        }
    }
};

