#include "InMemoryLoanRepository.h"

void InMemoryLoanRepository ::add(const Loan &loan)
{
    borrowedISBNs.insert(loan.getISBN());
}

bool InMemoryLoanRepository::isBookBorrowed(const std ::string &isbn) const
{
    if (borrowedISBNs.find(isbn) != borrowedISBNs.end())
    {
        return true;
    }
    return false;
}