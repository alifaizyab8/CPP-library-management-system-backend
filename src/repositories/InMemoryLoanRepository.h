#pragma once
#include <unordered_set>

#include "LoanRepository.h"
class InMemoryLoanRepository : public LoanRepository
{
private:
    std::unordered_set<std::string> borrowedISBNs;

public:
    void add(const Loan &loan) override;
    bool isBookBorrowed(const std::string &isbn) const override;
};