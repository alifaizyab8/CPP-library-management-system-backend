#pragma once
#include <string>
#include "../domain/Book.h"

class BookRepository
{
public:
    virtual ~BookRepository() = default;
    virtual void add(const Book &book) = 0;
    virtual const Book *findByISBN(const std ::string &isbn) const = 0;
};