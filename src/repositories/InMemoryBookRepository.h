#pragma once
#include <unordered_map>
#include "BookRepository.h"

class InMemoryBookRepository : public BookRepository
{

private:
    std ::unordered_map<std::string, Book> books;

public:
    void add(const Book &book) override;
    const Book *findByISBN(const std::string &isbn) const override;
};