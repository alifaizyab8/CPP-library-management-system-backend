#pragma once
#include <string>
#include <utility>
#include <optional> // used for data that might be null 

class Loan
{
private:
    std::string id; // Unique ID will be generated for the loan transaction
    std::string isbn;
    std::string userID;
    std::string issueDate;                 // Strictly follow format YYYY-MM-DD
    std::optional<std::string> returnDate; // Empty if not returned yet
public:
    ~Loan() = default;
    Loan(std ::string id, std::string isbn, std ::string userID, std::string issueDate);

    void markReturned(std::string date);
    bool isActive() const; // Returns true if return date is empty meaning book is still issued

    const std::string &getID() const;
    const std::string &getISBN() const;
    const std::string &getUserID() const;
    const std::string &getIssueDate() const;
    std ::optional<std::string> getReturnDate() const;
};