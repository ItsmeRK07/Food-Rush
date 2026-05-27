#pragma once
#include <string>
#include <iostream>
#include <iomanip>

// ============================================================================
// Display - Static Utility Class for Terminal UI
// OOP Concept: Abstraction
// Provides a consistent, centralized interface for all terminal output
// formatting. Encapsulates display logic so callers never worry about
// formatting details like divider widths, emoji prefixes, or alignment.
// ============================================================================
class Display {
public:
    static void showHeader(const std::string& title);
    static void showSubHeader(const std::string& title);
    static void showDivider();
    static void showSuccess(const std::string& message);
    static void showError(const std::string& message);
    static void showInfo(const std::string& message);
    static void showWarning(const std::string& message);
    static void pressEnterToContinue();
    static void clearScreen();
    static void showWelcomeBanner();
};
