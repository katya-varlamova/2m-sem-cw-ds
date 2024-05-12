#include "User.h"
User::User(
        const std::string& name,
        const std::string& login,
        const std::string& email,
        const std::string& mobilePhone,
        const std::string& lastName,
        const std::string& password,
        const std::string& role      
)
  : name( name )
  , login( login )
  , email( email )
  , mobilePhone( mobilePhone )
  , lastName (lastName)
  , password (password)
  , role (role)
{
}