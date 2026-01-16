#pragma once
#include "repositories/BookRepository.h"
#include "repositories/LoanRepository.h"
#include "repositories/UserRepository.h"

class BorrowService
{
private:
    BookRepository &bookRepo;
    LoanRepository &loanRepo;
    UserRepository &userRepo;

public:
    BorrowService(BookRepository &bookRepo, LoanRepository &loanRepo, UserRepository &userRepo);
    void borrowBook(const std ::string &isbn, const std::string &userID);
};
