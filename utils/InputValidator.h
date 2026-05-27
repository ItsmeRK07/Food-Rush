#pragma once
#include <string>
#include <iostream>
#include <limits>

// ============================================================================
// InputValidator - Static Utility Class
// OOP Concept: Abstraction
// Demonstrates class-level encapsulation with a purely static interface.
// All methods are static, providing a namespace-like grouping of related
// input validation functions with no instantiation required.
// ============================================================================
class InputValidator {
public:
    static int getValidInt(const std::string& prompt, int min, int max);
    static double getValidDouble(const std::string& prompt, double min, double max);
    static std::string getValidString(const std::string& prompt, int minLength = 1);
    static bool confirmAction(const std::string& prompt);
};
