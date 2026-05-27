#pragma once
#include <iostream>
#include "User.h" // Inherits from User

class DeliveryPartner : public User {
private:
    bool isAvailable;

public:
    DeliveryPartner(std::string uName, std::string pass) 
        : User(uName, pass), isAvailable(true) {}

    void setAvailability(bool status) { isAvailable = status; }

    void displayDashboard() override {
        std::cout << "Welcome Partner: " << username << " | Status: " 
                  << (isAvailable ? "Available" : "Busy") << "\n";
    }
};