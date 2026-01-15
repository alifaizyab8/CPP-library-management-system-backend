#include "Book.h"

#include <utility>

Book ::Book(std ::string title, std ::string author, std ::string isbn)
    : title(std::move(title)), author(std ::move(author)), isbn(std ::move(isbn)) {}

const std ::string &Book ::getTitle() const
{
    return title;
}
const std ::string &Book ::getAuthor() const
{
    return author;
}
const std ::string &Book ::getISBN() const
{
    return isbn;
}
