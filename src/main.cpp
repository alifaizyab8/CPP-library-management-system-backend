#include <iostream>
#include <memory>
#include "repositories/SqliteBookRepository.h"
#include "database/Database.h"
#include "repositories/InMemoryBookRepository.h"
#include "repositories/InMemoryLoanRepository.h"
#include "repositories/InMemoryUserRepository.h"
#include "services/BorrowService.h"

int main()
{
    auto db = std::make_shared<Database>("Library.db");
    SqliteBookRepository repo(db);
    std::cout << "--- Adding a Book to SQLite ---\n";
    Book book(
        "Clean Code",
        "Robert C. Martin",
        "9780132350884");

    repo.add(book);
    std::cout << "Book saved to database file successfully.\n\n";

    std::cout << "--- Retrieving Book from SQLite ---\n";
    const Book *b1 = repo.findByISBN("9780132350884");
    
    if (b1)
    {
        std::cout << "Found: " << b1->getTitle() << " by " << b1->getAuthor() << "\n";
    }
    else 
    {
        std::cout << "Book not found.\n";
    }

    return 0;
    

}