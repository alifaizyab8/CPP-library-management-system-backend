#include "Loan.h"

Loan ::Loan(std::string id, std ::string isbn, std::string userID, std::string issueDate)
    : id(std::move(id)), isbn(std::move(isbn)), userID(std::move(userID)), issueDate(std::move(issueDate)), returnDate(std::nullopt) {}

const std::string &Loan::getID() const
{
    return id;
}

const std::string &Loan ::getISBN() const
{
    return isbn;
}
const std::string &Loan ::getUserID() const
{
    return userID;
}
const std::string &Loan::getIssueDate() const
{
    return issueDate;
}
std::optional<std::string> Loan::getReturnDate() const
{
    return returnDate;
}
bool Loan::isActive() const
{
    // returnDate.has_value() is true when a return date is present
    return !returnDate.has_value();
}
void Loan::markReturned(std::string date)
{
    returnDate = std::move(date);
}