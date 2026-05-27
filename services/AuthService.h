#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "../models/Customer.h"

// Encapsulation: File I/O hidden behind service interface
// RAII: Smart pointers for memory safety
class AuthService {
private:
    std::string dbFile;

public:
    AuthService();
    bool registerCustomer(const std::string& username, const std::string& password);
    std::unique_ptr<Customer> loginCustomer(const std::string& username, const std::string& password);
    bool isUsernameTaken(const std::string& username);
    void updateWalletBalance(const std::string& username, double newBalance);
};