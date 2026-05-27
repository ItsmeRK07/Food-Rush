#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "../models/Customer.h"

class AuthService {
private:
    std::string dbFile = "users.txt";

public:
    // 1. Register a new customer
    void registerCustomer(std::string username, std::string password) {
        // ios::app opens the file in "append" mode so we don't overwrite existing data
        std::ofstream file(dbFile, std::ios::app); 
        
        if (file.is_open()) {
            file << "Customer " << username << " " << password << " 0.0\n";
            std::cout << "✅ Registration successful for " << username << "!\n";
            file.close();
        } else {
            std::cout << "❌ Error connecting to database.\n";
        }
    }

    // 2. Login an existing customer
    std::unique_ptr<Customer> loginCustomer(std::string inputUser, std::string inputPass) {
        std::ifstream file(dbFile);
        std::string role, dbUser, dbPass;
        double balance;

        if (file.is_open()) {
            while (file >> role >> dbUser >> dbPass >> balance) {
                if (role == "Customer" && dbUser == inputUser && dbPass == inputPass) {
                    std::cout << "✅ Login successful!\n";
                    
                    // Create a smart pointer for memory safety
                    auto loggedInUser = std::make_unique<Customer>(dbUser, dbPass);
                    loggedInUser->addFunds(balance); // Restore their saved balance
                    
                    file.close();
                    return loggedInUser; 
                }
            }
            file.close();
        }
        std::cout << "❌ Invalid username or password.\n";
        return nullptr;
    }
};