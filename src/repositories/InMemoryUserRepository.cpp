#include "InMemoryUserRepository.h"

void InMemoryUserRepository::add(const User &user)
{
    users.emplace(user.getID(), user);
}

const User *InMemoryUserRepository::getUserByID(const std::string &id) const
{
    auto target = users.find(id);
    if (target != users.end())
    {
        return &target->second;
    }
    return nullptr;
}