#include "SqliteBookRepository.h"
#include <sqlite3.h>
#include <stdexcept>

SqliteBookRepository ::SqliteBookRepository(std::shared_ptr<Database> db)
    : db(std::move(db))
{
    createTableIfNotExists();
}
void SqliteBookRepository ::createTableIfNotExists() const
{
    const char *sqlCode =
        "CREATE TABLE IF NOT EXISTS books("
        "isbn TEXT PRIMARY KEY, "
        "title TEXT NOT NULL, "
        "author TEXT NOT NULL);";

    char *errorMsg = nullptr;
    int rc = sqlite3_exec(db->getDbHandle(), sqlCode, nullptr, nullptr, &errorMsg);
    if (rc != SQLITE_OK)
    {
        std::string err = errorMsg;
        sqlite3_free(errorMsg);
        throw std::runtime_error("Failed to create table :" + err);
    }
}

void SqliteBookRepository::add(const Book &book)
{
    const char *sqlCode =
        "INSERT OR REPLACE INTO books (isbn, title, author) "
        "VALUES (?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare INSERT statement");
    }
    sqlite3_bind_text(stmt, 1, book.getISBN().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute Insertion");
    }
    sqlite3_finalize(stmt);
    cache.emplace(book.getISBN(), book);
}

const Book *SqliteBookRepository::findByISBN(const std::string &isbn) const
{
    auto target = cache.find(isbn);
    if (target != cache.end())
    {
        return &target->second;
    }

    const char *sqlCode =
        "SELECT isbn, title, author FROM books WHERE isbn = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare SELECT statement");
    }
    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book book(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));

        sqlite3_finalize(stmt);
        // also store in cache

        auto result = cache.emplace(isbn, std::move(book));
        return &result.first->second;
    }
    sqlite3_finalize(stmt);
    return nullptr;
}