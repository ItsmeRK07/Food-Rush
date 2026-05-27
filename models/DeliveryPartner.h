#pragma once
#include <iostream>
#include <iomanip>
#include "User.h"

// ============================================================================
// DeliveryPartner - Derived from User
// OOP Concept: Inheritance - inherits authentication from User base class.
// OOP Concept: Polymorphism - overrides displayDashboard() and toString()
//              so a User* pointing to a DeliveryPartner resolves correctly.
// ============================================================================
class DeliveryPartner : public User {
private:
    bool isAvailable;
    double rating;
    int totalDeliveries;
    int currentOrderId;

public:
    DeliveryPartner(const std::string& uName, const std::string& pass);

    // Getters - Encapsulation
    bool getIsAvailable() const;
    double getRating() const;
    int getTotalDeliveries() const;
    int getCurrentOrderId() const;

    // Delivery lifecycle
    void assignOrder(int orderId);
    void completeDelivery();
    void addRating(double newRating);

    // Polymorphism - override base class virtual functions
    void displayDashboard() override;
    std::string toString() const override;
};