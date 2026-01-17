#include <iostream>
#include <memory>
#include "repositories/SqliteBookRepository.h"
#include "database/Database.h"
#include "repositories/InMemoryBookRepository.h"
#include "repositories/InMemoryLoanRepository.h"
#include "repositories/InMemoryUserRepository.h"
#include "services/BorrowService.h"

int main()
{
    auto db = std::make_shared<Database>("Library.db");
    SqliteBookRepository bookRepo(db);
    

}