#include "SqliteLoanRepository.h"
#include <sqlite3.h>
#include <stdexcept>

SqliteLoanRepository::SqliteLoanRepository(std::shared_ptr<Database> db)
    : db(std::move(db))
{
    createTableIfNotExists();
}

void SqliteLoanRepository::createTableIfNotExists() const
{
    const char *sqlCode =
        "CREATE TABLE IF NOT EXISTS loans("
        "id TEXT PRIMARY KEY, "
        "book_isbn TEXT NOT NULL, "
        "user_id TEXT NOT NULL, "
        "issue_date TEXT NOT NULL, "
        "return_date TEXT" // Can be NULL
        ");";

    char *errorMsg = nullptr;
    if (sqlite3_exec(db->getDbHandle(), sqlCode, nullptr, nullptr, &errorMsg) != SQLITE_OK)
    {
        std::string err = errorMsg;
        sqlite3_free(errorMsg);
        throw std::runtime_error("Failed to create loans table: " + err);
    }
}

void SqliteLoanRepository::add(const Loan &loan)
{
    const char *sqlCode = "INSERT INTO loans (id, book_isbn, user_id, issue_date, return_date) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare Loan INSERT");

    sqlite3_bind_text(stmt, 1, loan.getID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, loan.getISBN().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, loan.getUserID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, loan.getIssueDate().c_str(), -1, SQLITE_TRANSIENT);

    if (loan.getReturnDate().has_value())
    {
        sqlite3_bind_text(stmt, 5, loan.getReturnDate().value().c_str(), -1, SQLITE_TRANSIENT);
    }
    else
    {
        sqlite3_bind_null(stmt, 5); // Store as SQL NULL
    }

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute Loan Insertion");
    }
    sqlite3_finalize(stmt);
}

std::vector<Loan> SqliteLoanRepository::getActiveLoansByUserId(const std::string &userId) const
{
    std::vector<Loan> loans;
    // Find all the loans where user_id is found AND return_date IS NULL
    const char *sqlCode = "SELECT id, book_isbn, user_id, issue_date, return_date FROM loans WHERE user_id = ? AND return_date IS NULL;";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare Active Loan SELECT");

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string id = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        std::string isbn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string userID = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string issue = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        Loan loan(id, isbn, userID, issue);

        // Check if return date exists (it shouldn't in this query, but good practice)
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL)
        {
            loan.markReturned(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
        }
        loans.push_back(std::move(loan));
    }
    sqlite3_finalize(stmt);
    return loans;
}

std::optional<Loan> SqliteLoanRepository::getLoanById(const std::string &loanId) const
{
    const char *sqlCode = "SELECT id, book_isbn, user_id, issue_date, return_date FROM loans WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_text(stmt, 1, loanId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string id = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        std::string isbn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string uid = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string issueDate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        Loan loan(id, isbn, uid, issueDate);
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL)
        {
            loan.markReturned(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
        }
        sqlite3_finalize(stmt);
        return loan;
    }
    sqlite3_finalize(stmt);
    return std::nullopt;
}

void SqliteLoanRepository::update(const Loan &loan)
{
    // Used to set return date
    const char *sqlCode = "UPDATE loans SET return_date = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare Loan UPDATE");

    if (loan.getReturnDate().has_value())
    {
        sqlite3_bind_text(stmt, 1, loan.getReturnDate().value().c_str(), -1, SQLITE_TRANSIENT);
    }
    else
    {
        sqlite3_bind_null(stmt, 1);
    }
    sqlite3_bind_text(stmt, 2, loan.getID().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to update Loan");
    }
    sqlite3_finalize(stmt);
}

std::optional<Loan> SqliteLoanRepository::getActiveLoanByISBN(const std::string &isbn) const
{

    const char *sqlCode = "SELECT id, book_isbn, user_id, issue_date, return_date FROM loans WHERE book_isbn = ? AND return_date IS NULL;";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string id = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        std::string bookIsbn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string userId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string issueDate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        Loan loan(id, bookIsbn, userId, issueDate);
        // We don't check return_date column here because we specifically queried for NULL

        sqlite3_finalize(stmt);
        return loan;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}