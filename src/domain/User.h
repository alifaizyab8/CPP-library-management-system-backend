#pragma once
#include <string>
enum class Role
{
    Student,
    Librarian
};

class User
{
private:
    std::string id;
    std::string name;
    Role role;

public:
    User(std::string id,std::string name, Role role);
    const std::string& getID()const;
    const std::string& getName()const;
    Role  getRole()const;
};