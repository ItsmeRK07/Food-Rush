#include "AuthService.h"
#include "../utils/FoodRushException.h"
#include <iomanip>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

AuthService::AuthService() : dbFile("data/users.txt") {
    MKDIR("data");
}

bool AuthService::registerCustomer(const std::string& username, const std::string& password) {
    if (password.length() < 4) {
        throw AuthException("Password must be at least 4 characters long!");
    }
    if (isUsernameTaken(username)) {
        throw AuthException("Username '" + username + "' is already taken!");
    }
    std::ofstream file(dbFile, std::ios::app);
    if (!file.is_open()) {
        throw AuthException("Could not open database file for registration!");
    }
    file << "Customer " << username << " " << password << " "
         << std::fixed << std::setprecision(2) << 0.00 << "\n";
    file.close();
    return true;
}

std::unique_ptr<Customer> AuthService::loginCustomer(const std::string& username, const std::string& password) {
    std::ifstream file(dbFile);
    if (!file.is_open()) {
        throw AuthException("No users registered yet! Please register first.");
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string role, fileUsername, filePassword;
        double balance;
        if (iss >> role >> fileUsername >> filePassword >> balance) {
            if (role == "Customer" && fileUsername == username && filePassword == password) {
                file.close();
                auto customer = std::make_unique<Customer>(username, password);
                customer->setWalletBalance(balance);
                return customer;
            }
        }
    }
    file.close();
    throw AuthException("Invalid username or password!");
}

bool AuthService::isUsernameTaken(const std::string& username) {
    std::ifstream file(dbFile);
    if (!file.is_open()) return false;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string role, fileUsername;
        if (iss >> role >> fileUsername) {
            if (fileUsername == username) { file.close(); return true; }
        }
    }
    file.close();
    return false;
}

void AuthService::updateWalletBalance(const std::string& username, double newBalance) {
    std::ifstream inFile(dbFile);
    if (!inFile.is_open()) {
        throw AuthException("Could not open database file for wallet update!");
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line)) { lines.push_back(line); }
    inFile.close();

    for (auto& l : lines) {
        std::istringstream iss(l);
        std::string role, fileUsername, filePassword;
        double balance;
        if (iss >> role >> fileUsername >> filePassword >> balance) {
            if (fileUsername == username) {
                std::ostringstream oss;
                oss << role << " " << fileUsername << " " << filePassword << " "
                    << std::fixed << std::setprecision(2) << newBalance;
                l = oss.str();
                break;
            }
        }
    }

    std::ofstream outFile(dbFile, std::ios::trunc);
    if (!outFile.is_open()) {
        throw AuthException("Could not write to database file for wallet update!");
    }
    for (const auto& l : lines) { outFile << l << "\n"; }
    outFile.close();
}
