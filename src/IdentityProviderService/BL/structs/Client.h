#pragma once

#include <string>

struct Client
{
    Client() = default;
    Client(
      int client_id, const std::string& client_secret) 
      : client_id( client_id ), client_secret (client_secret) 
      { }
    int client_id;
    std::string client_secret;
    bool operator==( const Client& l ) const
    {
        return l.client_id == this->client_id && l.client_secret == this->client_secret;
    }
};