#pragma once
#include <string>
#include <iostream>

// ============================================================================
// User - Abstract Base Class
// OOP Concept: Abstraction - pure virtual displayDashboard() forces subclasses
//              to provide their own implementation.
// OOP Concept: Polymorphism - virtual functions enable runtime dispatch.
//              A User* pointer can call the correct derived version.
// OOP Concept: Inheritance - serves as the base class for Customer and
//              DeliveryPartner, sharing common authentication logic.
// ============================================================================
class User {
protected:
    std::string username;
    std::string password;

public:
    User(const std::string& uName, const std::string& pass);
    virtual ~User() = default;

    // Getters - Encapsulation
    std::string getUsername() const;
    std::string getPassword() const;
    bool verifyPassword(const std::string& pass) const;

    // Pure virtual function - Abstraction (makes User abstract)
    virtual void displayDashboard() = 0;

    // Virtual function - Polymorphism (can be overridden by derived classes)
    virtual std::string toString() const;

    // Operator Overloading with Polymorphism
    // Uses virtual toString() so output adapts to the actual derived type
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};