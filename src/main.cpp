#include <iostream>
#include "repositories/InMemoryBookRepository.h"
#include "repositories/InMemoryLoanRepository.h"
#include"repositories/InMemoryUserRepository.h"
#include "services/BorrowService.h"

int main()
{
    InMemoryBookRepository bookRepo;
    InMemoryLoanRepository loanRepo;
    InMemoryUserRepository userRepo;

    BorrowService service(bookRepo,loanRepo,userRepo);
    User student("u142","Ali",Role::Student);
    userRepo.add(student);
    Book book("Program","Ali","111");
    bookRepo.add(book);
    try
    {
        service.borrowBook("111","u142");
        std::cout << "Book borrowed successfully\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}