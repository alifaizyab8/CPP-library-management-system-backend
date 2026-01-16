#include "InMemoryBookRepository.h"

void InMemoryBookRepository ::add(const Book &book)
{
    books.emplace(book.getISBN(), book);
}

const Book *InMemoryBookRepository ::findByISBN(const std::string &isbn) const
{
    auto target = books.find(isbn);
    if (target != books.end())
    {
        return &target->second;
    }
    return nullptr;
}