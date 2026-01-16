#include "BorrowService.h"
#include <stdexcept>
BorrowService ::BorrowService(BookRepository &bookRepo, LoanRepository &loanRepo, UserRepository &userRepo)
    : bookRepo(bookRepo), loanRepo(loanRepo), userRepo(userRepo) {}

void BorrowService::borrowBook(const std::string &isbn, const std::string &userID)

{
    const User *user = userRepo.getUserByID(userID);
    if (!user)
    {
        throw std::runtime_error("User not Found");
    }

    const Book *book = bookRepo.findByISBN(isbn);
    if (!book)
    {
        throw std::runtime_error("Book does not exist");
    }
    if (loanRepo.isBookBorrowed(isbn))
    {
        throw std::runtime_error("Book already borrowed");
    }

    Loan loan(isbn, userID);
    loanRepo.add(loan);
}