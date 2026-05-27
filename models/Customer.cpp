#include "Customer.h"
#include <sstream>

// ============================================================================
// Customer Implementation
// OOP Concept: Inheritance - calls User(uName, pass) in the initializer list.
// OOP Concept: Polymorphism - displayDashboard() and toString() override the
//              base class versions, enabling correct behavior through User*.
// ============================================================================

// Constructor: delegates to User base, initializes wallet to zero
Customer::Customer(const std::string& uName, const std::string& pass)
    : User(uName, pass), walletBalance(0.0) {}

// Add funds with validation
void Customer::addFunds(double amount) {
    if (amount <= 0) {
        std::cout << "  \xE2\x9D\x8C Amount must be greater than zero.\r\n";
        return;
    }
    walletBalance += amount;
    std::cout << "  \xE2\x9C\x85 \xE2\x82\xB9" << std::fixed << std::setprecision(2) << amount
              << " added. New balance: \xE2\x82\xB9" << walletBalance << "\r\n";
}

// Deduct funds - returns false if insufficient balance
bool Customer::deductFunds(double amount) {
    if (amount > walletBalance) {
        return false;
    }
    walletBalance -= amount;
    return true;
}

double Customer::getWalletBalance() const { return walletBalance; }

// Composition: Order is stored by value in the history vector
void Customer::addOrder(const Order& order) {
    orderHistory.push_back(order);
}

const std::vector<Order>& Customer::getOrderHistory() const { return orderHistory; }

// Display all past orders
void Customer::displayOrderHistory() const {
    if (orderHistory.empty()) {
        std::cout << "\r\n  \xF0\x9F\x93\xAD No orders yet. Time to order something delicious!\r\n";
        return;
    }

    std::cout << "\r\n  " << std::string(50, '=') << "\r\n";
    std::cout << "   \xF0\x9F\x93\x9C Order History (" << orderHistory.size() << " orders)\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
    std::cout << "  " << std::left << std::setw(7) << "#ID"
              << " | " << std::setw(20) << "Restaurant"
              << " | " << std::setw(10) << "Total"
              << " | Status\r\n";
    std::cout << "  " << std::string(50, '-') << "\r\n";

    for (size_t i = 0; i < orderHistory.size(); ++i) {
        orderHistory[i].displaySummary();
    }
    std::cout << "  " << std::string(50, '=') << "\r\n";
}

// Polymorphism: Customer-specific dashboard override
void Customer::displayDashboard() {
    std::cout << "\r\n  " << std::string(50, '=') << "\r\n";
    std::cout << "   \xF0\x9F\x91\xA4 Customer Dashboard\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
    std::cout << "  Username       : " << username << "\r\n";
    std::cout << "  \xF0\x9F\x92\xB0 Wallet Balance : \xE2\x82\xB9"
              << std::fixed << std::setprecision(2) << walletBalance << "\r\n";
    std::cout << "  \xF0\x9F\x93\xA6 Total Orders   : " << orderHistory.size() << "\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
}

// Polymorphism: Customer-specific toString override
std::string Customer::toString() const {
    std::ostringstream oss;
    oss << "Customer: " << username << " | Wallet: \xE2\x82\xB9"
        << std::fixed << std::setprecision(2) << walletBalance;
    return oss.str();
}
