#pragma once
#include "UserRepository.h"
#include <unordered_map>
class InMemoryUserRepository : public UserRepository
{
private:
    std::unordered_map<std::string, User> users;

public:
    void add(const User &user) override;
    const User *getUserByID(const std::string &id) const override;
};