#pragma once
#include"UserRepository.h"
#include"database/Database.h"
#include"unordered_map"
#include"memory"


class SqliteUserRepository : public UserRepository
{
    private: 
        std::shared_ptr<Database> db;
        mutable std:: unordered_map<std:: string, User> cache;
        void createTableIfNotExists() const;

    public:
        explicit SqliteUserRepository(std:: shared_ptr<Database> db);
        void add(const User &User) override;
        const User *getUserByID(const std::string &id) const override;
};