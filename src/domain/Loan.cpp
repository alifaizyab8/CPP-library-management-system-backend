#include "Loan.h"

Loan ::Loan(std ::string isbn, std::string userID)
    : isbn(std::move(isbn)), userID(std::move(userID)) {}
const std::string &Loan ::getISBN() const
{
    return isbn;
}
const std::string &Loan ::getUserID() const
{
    return userID;
}