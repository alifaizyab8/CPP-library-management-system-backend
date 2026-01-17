#include<sqlite3.h>
#include <string>
class Database
{
public:
    explicit Database(const std::string &path);
    ~Database();

    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;
    sqlite3 *getDbHandle() const;

private:
    sqlite3 *db;
};