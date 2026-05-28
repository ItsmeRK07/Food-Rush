#pragma once
#include <stdexcept>
#include <string>

// ============================================================================
// FoodRushException - Custom Exception Hierarchy
// OOP Concept: Inheritance - each exception type derives from FoodRushException,
//              which itself derives from std::runtime_error.
// OOP Concept: Polymorphism - catch handlers can catch by base or derived type.
// Replaces error return codes with a clean, type-safe exception hierarchy.
// ============================================================================

class FoodRushException : public std::runtime_error {
public:
    explicit FoodRushException(const std::string& msg) : std::runtime_error(msg) {}
};

// Thrown for authentication failures: bad credentials, duplicate username, etc.
class AuthException : public FoodRushException {
public:
    explicit AuthException(const std::string& msg) : FoodRushException(msg) {}
};

// Thrown for order workflow errors: empty cart, insufficient funds, cancellation issues
class OrderException : public FoodRushException {
public:
    explicit OrderException(const std::string& msg) : FoodRushException(msg) {}
};

// Thrown for routing errors: missing locations, no path available
class RouteException : public FoodRushException {
public:
    explicit RouteException(const std::string& msg) : FoodRushException(msg) {}
};

// Thrown for input/data validation errors: invalid cart operations, bad input
class ValidationException : public FoodRushException {
public:
    explicit ValidationException(const std::string& msg) : FoodRushException(msg) {}
};
