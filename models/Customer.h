#pragma once
#include <iostream>
#include "User.h"

class Customer : public User {
private:
    double walletBalance;

public:
    Customer(std::string uName, std::string pass) : User(uName, pass), walletBalance(0.0) {}

    void addFunds(double amount) { walletBalance += amount; }
    double getWalletBalance() const { return walletBalance; }

    void displayDashboard() override {
        std::cout << "Customer Dashboard for " << username << "\n";
        std::cout << "Wallet Balance: $" << walletBalance << "\n";
    }
};