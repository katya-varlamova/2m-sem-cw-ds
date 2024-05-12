#ifndef BASE_SECTIONS_H
#define BASE_SECTIONS_H

#include <string>

const std::string LogFileSection = "LogFile";
const std::string JwksSection = "jwks";
const std::string PrivateKeysSection = "private_keys";
const std::string CountKeysSection = "keys_count";
const std::string ClientIDSection = "client_id";
const std::string ClientSecretSection = "client_secret";
const std::string AuthURLSection = "auth_url";
const std::string ScopeStringSection = "scope_string";
const std::string ServerSection = "Server";
const std::string PortSection = "Port";
const std::string ClientsSection = "Clients";
const std::string ClientHostSection = "Host";
const std::string ClientPortSection = "Port";

const std::string DbHostSection = "DB_HOST";
const std::string DbNameSection = "DB_NAME";
const std::string DbPortSection = "DB_PORT";
const std::string DbUserSection = "DB_USER";
const std::string DbUserPasswordSection = "DB_USER_PASSWORD";

const std::string UsedCriterions = "UsedCriterions";

const std::string AuthSection = "Auth";
const std::string JWTTokenLifeTime = "JWTTokenLifeTime";
const std::string JWTAlgorithm = "JWTAlgorithm";
const std::string JWTSecret = "JWTSecret";

#endif // BASE_SECTIONS_H
