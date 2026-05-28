#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "User.h"
#include "Order.h"

// ============================================================================
// Customer - Derived from User
// OOP Concept: Inheritance - inherits authentication from User base class.
// OOP Concept: Composition - contains a vector of Orders (order history).
// OOP Concept: Polymorphism - overrides displayDashboard() and toString()
//              so a User* pointing to a Customer calls Customer's versions.
// ============================================================================
class Customer : public User {
private:
    double walletBalance;
    std::vector<Order> orderHistory;

public:
    Customer(const std::string& uName, const std::string& pass);

    // Wallet management
    void addFunds(double amount);
    bool deductFunds(double amount);
    void setWalletBalance(double balance);  // Silent setter for loading from file
    double getWalletBalance() const;

    // Order history (Composition)
    void addOrder(const Order& order);
    const std::vector<Order>& getOrderHistory() const;
    void displayOrderHistory() const;

    // Polymorphism - override base class virtual functions
    void displayDashboard() override;
    std::string toString() const override;
};