#include <iostream>
#include "domain/Book.h"
#include "repositories/InMemoryBookRepository.h"

int main()
{
    Book book(
        "Clean Code",
        "Robert C. Martin",
        "9780132350884");

    // std::cout << book.getTitle() << "\n";
    // std::cout << book.getAuthor() << "\n";
    // std::cout << book.getISBN() << "\n";

    InMemoryBookRepository repo;
    repo.add(book);

    const Book *b1;
    b1 = repo.findByISBN("9780132350884");
    if (b1)
    {
        std ::cout << b1->getTitle();
    }
}
