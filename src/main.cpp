#include <iostream>
#include "domain/Book.h"

int main()
{
    Book book(
        "Clean Code",
        "Robert C. Martin",
        "9780132350884");

    std::cout << book.getTitle() << "\n";
    std::cout << book.getAuthor() << "\n";
    std::cout << book.getISBN() << "\n";
}
