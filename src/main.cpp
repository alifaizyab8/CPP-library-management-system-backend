#include <iostream>
#include "repositories/InMemoryBookRepository.h"
#include "repositories/InMemoryLoanRepository.h"
#include "services/BorrowService.h"

int main()
{
    InMemoryBookRepository bookRepo;
    InMemoryLoanRepository loanRepo;

    BorrowService service(bookRepo,loanRepo);
    Book book("Program","Ali","111");
    bookRepo.add(book);
    try
    {
        service.borrowBook("111","User_1");
        service.borrowBook("111","User_2");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}