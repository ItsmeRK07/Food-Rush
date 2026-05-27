#include "InputValidator.h"
#include <algorithm>
#include <cctype>

// ============================================================================
// InputValidator Implementation
// OOP Concept: Abstraction - hides complexity of input validation behind
// a clean static interface. Callers never deal with cin edge cases.
// ============================================================================

int InputValidator::getValidInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  ❌ Invalid input. Please enter a number.\r\n";
            continue;
        }

        // Clear remaining characters in the buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (value < min || value > max) {
            std::cout << "  ❌ Please enter a value between " << min << " and " << max << ".\r\n";
            continue;
        }

        return value;
    }
}

double InputValidator::getValidDouble(const std::string& prompt, double min, double max) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  ❌ Invalid input. Please enter a number.\r\n";
            continue;
        }

        // Clear remaining characters in the buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (value < min || value > max) {
            std::cout << "  ❌ Please enter a value between " << min << " and " << max << ".\r\n";
            continue;
        }

        return value;
    }
}

std::string InputValidator::getValidString(const std::string& prompt, int minLength) {
    std::string value;
    while (true) {
        std::cout << prompt;

        // Clear any leftover characters before getline
        if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }
        std::getline(std::cin, value);

        // Trim leading whitespace
        size_t start = value.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            value = "";
        } else {
            // Trim trailing whitespace
            size_t end = value.find_last_not_of(" \t\r\n");
            value = value.substr(start, end - start + 1);
        }

        if (static_cast<int>(value.length()) < minLength) {
            std::cout << "  ❌ Input must be at least " << minLength << " character(s) long.\r\n";
            continue;
        }

        return value;
    }
}

bool InputValidator::confirmAction(const std::string& prompt) {
    char choice;
    std::cout << prompt << " (y/n): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return (choice == 'y' || choice == 'Y');
}
