#pragma once
#include <string>

struct bookLocation
{
    std::string aisle;
    std::string shelf;
    std::string bin;
};

class Book
{

private:
    // Considering that 1 ISBN = 1 BOOK for simple implementation
    std ::string title;
    std ::string author;
    std ::string isbn;
    bookLocation location;

public:
    ~Book() = default;
    Book(std ::string title, std ::string author, std ::string isbn, bookLocation location);
    // Getters
    const std ::string &getTitle() const;
    const std ::string &getAuthor() const;
    const std ::string &getISBN() const;
    const bookLocation &getLocation() const;
};