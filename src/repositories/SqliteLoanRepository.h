#pragma once
#include "LoanRepository.h"
#include "database/Database.h"
#include <memory>
#include <vector>

class SqliteLoanRepository : public LoanRepository
{
private:
    std::shared_ptr<Database> db;
    void createTableIfNotExists() const;

public:
    explicit SqliteLoanRepository(std::shared_ptr<Database> db);

    void add(const Loan &loan) override;

    std::vector<Loan> getActiveLoansByUserId(const std::string &userId) const override;

    std::optional<Loan> getLoanById(const std::string &loanId) const override;

    void update(const Loan &loan) override;
    std::optional<Loan> getActiveLoanByISBN(const std::string &isbn) const override;
};