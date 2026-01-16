#pragma once
#include <string>
#include "domain/User.h"
class UserRepository
{
public:
    virtual void add(const User &user) = 0;
    virtual const User *getUserByID(const std::string &id) const = 0;
};