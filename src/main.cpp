#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <optional>

// Domain Headers
#include "domain/Book.h"
#include "domain/User.h"
#include "domain/Loan.h"

// Repository Headers
#include "repositories/SqliteBookRepository.h"
#include "repositories/SqliteUserRepository.h"
#include "repositories/SqliteLoanRepository.h"

// Database Header
#include "database/Database.h"

void testBookRepository(std::shared_ptr<Database> db)
{
    std::cout << "--- Testing Book Repository ---" << std::endl;
    SqliteBookRepository bookRepo(db);

    // 1. Create a Book Object
    // Aisle A1, Shelf S1, Bin B1
    bookLocation loc{"A1", "S1", "B1"};
    Book newBook("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", loc);

    // 2. Add Book to Database
    try
    {
        bookRepo.add(newBook);
        std::cout << "Book added successfully." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to add book: " << e.what() << std::endl;
        return;
    }

    // 3. Find Book by ISBN
    const Book *retrievedBook = bookRepo.findByISBN("9780743273565");
    if (retrievedBook)
    {
        std::cout << "Book found: \"" << retrievedBook->getTitle()
                  << "\" by " << retrievedBook->getAuthor() << std::endl;

        // Verify Location
        if (retrievedBook->getLocation().aisle == "A1")
        {
            std::cout << "Location data verified." << std::endl;
        }
        else
        {
            std::cerr << "Location data mismatch." << std::endl;
        }
    }
    else
    {
        std::cerr << "Book not found after insertion." << std::endl;
    }
}

void testUserRepository(std::shared_ptr<Database> db)
{
    std::cout << "\n--- Testing User Repository ---" << std::endl;
    SqliteUserRepository userRepo(db);

    // 1. Create a User Object
    User newUser("U001", "John Doe", Role::Student);

    // 2. Add User to Database
    try
    {
        userRepo.add(newUser);
        std::cout << "User added successfully." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to add user: " << e.what() << std::endl;
        std::cerr << "       (Check SqliteUserRepository.cpp for syntax errors!)" << std::endl;
        return;
    }

    // 3. Retrieve User by ID
    const User *retrievedUser = userRepo.getUserByID("U001");
    if (retrievedUser)
    {
        std::cout << "User found: " << retrievedUser->getName() << std::endl;
    }
    else
    {
        std::cerr << "User not found after insertion." << std::endl;
    }
}

void testLoanRepository(std::shared_ptr<Database> db)
{
    std::cout << "\n--- Testing Loan Repository ---" << std::endl;
    SqliteLoanRepository loanRepo(db);

    std::string loanId = "L100";
    std::string isbn = "9780743273565"; // Must match the book we added earlier
    std::string userId = "U001";        // Must match the user we added earlier
    std::string issueDate = "2023-10-01";

    Loan newLoan(loanId, isbn, userId, issueDate);

    // 1. Add Loan
    try
    {
        loanRepo.add(newLoan);
        std::cout << "Loan added successfully." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to add loan: " << e.what() << std::endl;
        return;
    }

    // 2. Check Active Loans by User
    auto userLoans = loanRepo.getActiveLoansByUserId(userId);
    if (!userLoans.empty())
    {
        std::cout << "Found " << userLoans.size() << " active loan(s) for user " << userId << "." << std::endl;
    }
    else
    {
        std::cerr << "No active loans found for user." << std::endl;
    }

    // 3. Check Active Loan by ISBN
    auto activeLoanOpt = loanRepo.getActiveLoanByISBN(isbn);
    if (activeLoanOpt.has_value())
    {
        std::cout << "Active loan found by ISBN." << std::endl;

        // 4. Return the Book (Update Loan)
        Loan &activeLoan = *activeLoanOpt;
        activeLoan.markReturned("2023-10-15");

        try
        {
            loanRepo.update(activeLoan);
            std::cout << "Loan updated (book returned)." << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to update loan: " << e.what() << std::endl;
        }

        // 5. Verify it is no longer "Active"
        auto checkLoan = loanRepo.getActiveLoanByISBN(isbn);
        if (!checkLoan.has_value())
        {
            std::cout << "System correctly reports no active loan for this ISBN now." << std::endl;
        }
        else
        {
            std::cerr << "System still reports an active loan (Update failed)." << std::endl;
        }
    }
    else
    {
        std::cerr << "Could not find the active loan by ISBN." << std::endl;
    }
}

int main()
{
    try
    {
        //Connect to SQLite database file
        std::string dbPath = "library_test.db";
        auto db = std::make_shared<Database>(dbPath);
        std::cout << "Database connection established to '" << dbPath << "'." << std::endl;

        // Execute Tests
        testBookRepository(db);
        testUserRepository(db);
        testLoanRepository(db);
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}