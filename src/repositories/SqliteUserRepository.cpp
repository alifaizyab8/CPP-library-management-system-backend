#include "SqliteUserRepository.h"
#include "sqlite3.h"
#include <stdexcept>
SqliteUserRepository ::SqliteUserRepository(std::shared_ptr<Database> db)
    : db(std::move(db))
{
    createTableIfNotExists();
}

void SqliteUserRepository ::createTableIfNotExists() const
{
    const char *sqlCode =
        "CREATE TABLE IF NOT EXISTS users("
        "id TEXT PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "role INTEGER NOT NULL, "
        ");";
    char *errorMsg = nullptr;

    int status = sqlite3_exec(db->getDbHandle(), sqlCode, nullptr, nullptr, &errorMsg);
    if (status != SQLITE_OK)
    {
        std::string err = errorMsg;
        sqlite3_free(errorMsg);
        throw std::runtime_error("Failed to create table :" + err);
    }
}

void SqliteUserRepository::add(const User &user)
{
    const char *sqlCode = "INSERT OR REPLACE INTO users (id, name, role) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare User INSERT");

    sqlite3_bind_text(stmt, 1, user.getID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, static_cast<int>(user.getRole())); // Cast Enum to Int

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute User Insertion");
    }
    sqlite3_finalize(stmt);

    cache.erase(user.getID());
    cache.emplace(user.getID(), user);
}

const User *SqliteUserRepository::getUserByID(const std::string &id) const
{
    auto target = cache.find(id);
    if (target != cache.end())
        return &target->second;

    const char *sqlCode = "SELECT id, name, role FROM users WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db->getDbHandle(), sqlCode, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare User SELECT");

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        User user(
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
            static_cast<Role>(sqlite3_column_int(stmt, 2)));

        sqlite3_finalize(stmt);
        auto result = cache.emplace(id, std::move(user));
        return &result.first->second;
    }

    sqlite3_finalize(stmt);
    return nullptr;
}
