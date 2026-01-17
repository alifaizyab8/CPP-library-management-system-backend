#include "Database.h"
#include <stdexcept>

Database::Database(const std::string &path) : db(nullptr)
{
    int result = sqlite3_open(path.c_str(), &db);
    if (result != SQLITE_OK)
    {
        std::string errorMsg = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error("Failed to open database: " + errorMsg);
    }
}

Database::~Database()
{
    if (db)
    {
        sqlite3_close(db);
    }
}

sqlite3 *Database::getDbHandle() const
{
    return db;
}