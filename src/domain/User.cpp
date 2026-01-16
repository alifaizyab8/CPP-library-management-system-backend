#include <string>
#include "User.h"

User::User(std::string id, std::string name, Role role)
    : id(std::move(id)), name(std::move(name)), role(role) {}

const std::string &User::getID() const
{
    return id;
}

const std::string &User::getName() const
{
    return name;
}

Role User::getRole() const
{
    return role;
}