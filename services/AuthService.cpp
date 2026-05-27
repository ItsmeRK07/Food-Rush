#include "AuthService.h"
#include <iomanip>
#include <filesystem>

// Constructor: Initialize with data file path
// Encapsulation: Database file path is an implementation detail
AuthService::AuthService() : dbFile("data/users.txt") {
    // Ensure data directory exists (RAII-style resource management)
    std::filesystem::create_directories("data");
}

// Registers a new customer by appending to the data file
// Validates username uniqueness and password length before registration
bool AuthService::registerCustomer(const std::string& username, const std::string& password) {
    // Validate password length
    if (password.length() < 4) {
        std::cout << "\n  \u274C Password must be at least 4 characters long!" << std::endl;
        return false;
    }

    // Check if username already exists (Encapsulation: delegates to helper)
    if (isUsernameTaken(username)) {
        std::cout << "\n  \u274C Username '" << username << "' is already taken!" << std::endl;
        return false;
    }

    // Open file in append mode for adding new user
    std::ofstream file(dbFile, std::ios::app);
    if (!file.is_open()) {
        std::cout << "\n  \u274C Error: Could not open database file!" << std::endl;
        return false;
    }

    // Write user record: role username password balance
    file << "Customer " << username << " " << password << " "
         << std::fixed << std::setprecision(2) << 0.00 << "\n";
    file.close();

    std::cout << "\n  \u2705 Registration successful! Welcome, " << username << "!" << std::endl;
    return true;
}

// Authenticates a customer and returns a smart pointer to the Customer object
// RAII: Uses unique_ptr for automatic memory management
std::unique_ptr<Customer> AuthService::loginCustomer(const std::string& username, const std::string& password) {
    std::ifstream file(dbFile);
    if (!file.is_open()) {
        std::cout << "\n  \u274C Error: No users registered yet!" << std::endl;
        return nullptr;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string role, fileUsername, filePassword;
        double balance;

        if (iss >> role >> fileUsername >> filePassword >> balance) {
            // Only authenticate Customer role accounts
            if (role == "Customer" && fileUsername == username && filePassword == password) {
                file.close();
                // Polymorphism: Creating derived class object via smart pointer
                auto customer = std::make_unique<Customer>(username, password);
                customer->addFunds(balance);
                std::cout << "\n  \u2705 Login successful! Welcome back, " << username << "!" << std::endl;
                return customer;
            }
        }
    }

    file.close();
    std::cout << "\n  \u274C Invalid username or password!" << std::endl;
    return nullptr;
}

// Checks if a username is already registered in the database
// Encapsulation: File scanning logic hidden from callers
bool AuthService::isUsernameTaken(const std::string& username) {
    std::ifstream file(dbFile);
    if (!file.is_open()) {
        return false; // File doesn't exist yet, no users registered
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string role, fileUsername;
        if (iss >> role >> fileUsername) {
            if (fileUsername == username) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

// Updates a user's wallet balance in the persistent data file
// Reads all lines, modifies the target line, and writes everything back
// This ensures data integrity by performing an atomic-style update
void AuthService::updateWalletBalance(const std::string& username, double newBalance) {
    std::ifstream inFile(dbFile);
    if (!inFile.is_open()) {
        std::cout << "\n  \u274C Error: Could not open database file!" << std::endl;
        return;
    }

    // Read all lines into memory
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // Find and update the matching user's balance
    for (auto& l : lines) {
        std::istringstream iss(l);
        std::string role, fileUsername, filePassword;
        double balance;

        if (iss >> role >> fileUsername >> filePassword >> balance) {
            if (fileUsername == username) {
                // Rebuild the line with updated balance
                std::ostringstream oss;
                oss << role << " " << fileUsername << " " << filePassword << " "
                    << std::fixed << std::setprecision(2) << newBalance;
                l = oss.str();
                break;
            }
        }
    }

    // Write all lines back to file (overwrite mode)
    std::ofstream outFile(dbFile, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "\n  \u274C Error: Could not write to database file!" << std::endl;
        return;
    }

    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();
}
