#pragma once
#include "../repositories/BookRepository.h"
#include "../repositories/LoanRepository.h"

class BorrowService
{
private:
    BookRepository &bookRepo;
    LoanRepository &loanRepo;

public:
    BorrowService(BookRepository &bookRepo, LoanRepository &loanRepo);
    void borrowBook(const std ::string &isbn, const std::string &userID);
};
