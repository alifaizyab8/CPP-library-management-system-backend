#pragma once
#include <string>
#include "domain/Loan.h"
#include <vector>

class LoanRepository
{

public:
    virtual ~LoanRepository() = default;
    virtual void add(const Loan &loan) = 0;
    virtual std::vector<Loan> getActiveLoansByUserId(const std::string &userId) const = 0;
    virtual std::optional<Loan> getLoanById(const std::string &loanId) const = 0;
    virtual void update(const Loan &loan) = 0;

    virtual std::optional<Loan> getActiveLoanByISBN(const std::string &isbn) const = 0;
};