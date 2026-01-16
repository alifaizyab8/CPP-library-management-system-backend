#pragma once
#include <string>
#include "domain/Loan.h"

class LoanRepository
{

public:
    virtual ~LoanRepository() = default;
    virtual void add(const Loan &loan) = 0;
    virtual bool isBookBorrowed(const std::string &isbn) const = 0;
};