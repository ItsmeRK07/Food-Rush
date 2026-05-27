#include "User.h"

// ============================================================================
// User Implementation (Abstract Base Class)
// OOP Concept: Polymorphism - operator<< calls virtual toString(), so the
// correct derived version is invoked at runtime through the base reference.
// ============================================================================

// Constructor: initializes shared credentials
User::User(const std::string& uName, const std::string& pass)
    : username(uName), password(pass) {}

// --- Getters (Encapsulation) ---
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }

// Password verification - simple equality check
bool User::verifyPassword(const std::string& pass) const {
    return password == pass;
}

// Default toString - derived classes override for richer output
std::string User::toString() const {
    return "User: " + username;
}

// Operator Overloading: delegates to virtual toString() for polymorphic output
std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.toString();
    return os;
}
