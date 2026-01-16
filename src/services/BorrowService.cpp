#include "BorrowService.h"
#include <stdexcept>
BorrowService ::BorrowService(BookRepository &bookRepo, LoanRepository &loanRepo)
    : bookRepo(bookRepo), loanRepo(loanRepo) {}

void BorrowService::borrowBook(const std::string &isbn, const std::string &userID)

{
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