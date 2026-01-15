#pragma once
#include <string>

class Book
{

private:
    std ::string title;
    std ::string author;
    std ::string isbn;

public:
    Book(std ::string title, std ::string author, std ::string isbn);
    const std ::string &getTitle() const;
    const std ::string &getAuthor() const;
    const std ::string &getISBN() const;
};