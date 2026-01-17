#pragma once
#include "BookRepository.h"
#include "database/Database.h"
#include <unordered_map>
#include <memory>

class SqliteBookRepository : public BookRepository
{
private:
    std::shared_ptr<Database> db;
    // Cache Memory
    mutable std::unordered_map<std::string, Book> cache;
    void createTableIfNotExists() const;

public:
    explicit SqliteBookRepository(std::shared_ptr<Database> db);
    void add(const Book &book) override;
    const Book *findByISBN(const std::string &isbn) const override;
};