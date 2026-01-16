#pragma once
#include <string>
#include<utility>

class Loan
{
private:
    std::string isbn;
    std::string userID;

public:
    ~Loan() = default;
    Loan(std :: string isbn, std:: string userID);

    const std:: string& getISBN() const;
    const std:: string& getUserID() const;
    
};