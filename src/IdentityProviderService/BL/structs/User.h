#pragma once

#include <string>

struct User
{
    User() = default;
    User(
      const std::string& name,
      const std::string& login,
      const std::string& email,
      const std::string& mobilePhone,
      const std::string& lastName,
      const std::string& password,
      const std::string& role
    );
    std::string name, login, email;
    std::string mobilePhone, lastName, password, role;
    bool operator==( const User& l ) const
    {
        return l.name == this->name &&
               l.mobilePhone == this->mobilePhone && l.login == this->login &&
               l.email == this->email && l.password == this->password && l.lastName == this->lastName &&
               l.role == this->role;
    }
};