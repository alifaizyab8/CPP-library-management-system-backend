#include "SqliteBookRepository.h"
#include <sqlite3.h>
#include <stdexcept>

// This is a constructor of the Database Object, it takes a pointer to the database type object. The Pointer is shared meaning that multiple classes can use the same pointer/database
// It assigns the passed pointer to the class member db.
// Then this constructor calls the createTableIfNotExists function, this function ensures that the table is ready before use

SqliteBookRepository ::SqliteBookRepository(std::shared_ptr<Database> db)
    : db(std::move(db))
{
    createTableIfNotExists();
}
void SqliteBookRepository ::createTableIfNotExists() const
{
    // SQLite requires that the SQL must be passed as a trivial C-Styled String. So I had to use char*
    const char *sqlCode =
        "CREATE TABLE IF NOT EXISTS books("
        "isbn TEXT PRIMARY KEY, "
        "title TEXT NOT NULL, "
        "author TEXT NOT NULL, "
        "aisle TEXT, "
        "shelf TEXT, "
        "bin TEXT"
        ");";

    // SQLite stores error messages here if anything goes wrong
    char *errorMsg = nullptr;

    // This SQL function is used to execute SQL directly. It is commonly used to CREATE, DROP and execute simple queries
    // It returns an integer status code
    // The parameters required are Database Connection, SQL string, Callback, Callback Data, Address of Error Message Pointer

    int status = sqlite3_exec(db->getDbHandle(), sqlCode, nullptr, nullptr, &errorMsg);
    if (status != SQLITE_OK)
    {
        // If any error occured then copy the error message and free the SQLite Error Message Pointer
        std::string err = errorMsg;
        sqlite3_free(errorMsg);
        throw std::runtime_error("Failed to create table :" + err);
    }
}

void SqliteBookRepository::add(const Book &book)
{
    const char *sqlCode =
        "INSERT OR REPLACE INTO books (isbn, title, author, aisle, shelf, bin) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare INSERT statement");
    }
    // SQLITE_TRANSIENT tells SQLite to make its own private copy of the bound data because the original data may change or be destroyed.
    sqlite3_bind_text(stmt, 1, book.getISBN().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    // Binding Location Data
    sqlite3_bind_text(stmt, 4, book.getLocation().aisle.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, book.getLocation().shelf.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, book.getLocation().bin.c_str(), -1, SQLITE_TRANSIENT);

    // This must return SQLITE_DONE for a successful insert or replace
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        // Free the memory of the statement
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute Insertion");
    }
    // Free the memory of the statement
    sqlite3_finalize(stmt);
    // Remove old book if it exists in case of replace
    cache.erase(book.getISBN());
    // Normal addition in case of insertion
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
        "SELECT isbn, title, author, aisle, shelf, bin FROM books WHERE isbn = ?;";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare SELECT statement");
    }
    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        bookLocation location{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5))};
        Book book(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)),
            location);

        sqlite3_finalize(stmt);
        // also store in cache

        auto result = cache.emplace(isbn, std::move(book));
        return &book;
    }
    sqlite3_finalize(stmt);
    return nullptr;
}