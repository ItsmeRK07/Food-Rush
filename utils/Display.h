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
    // ANSI Color Constants
    static constexpr const char* RESET   = "\033[0m";
    static constexpr const char* RED     = "\033[31m";
    static constexpr const char* GREEN   = "\033[32m";
    static constexpr const char* YELLOW  = "\033[33m";
    static constexpr const char* CYAN    = "\033[36m";
    static constexpr const char* MAGENTA = "\033[35m";
    static constexpr const char* BOLD    = "\033[1m";
    static constexpr const char* DIM     = "\033[2m";

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
    static void showProgressBar(const std::string& label, int durationSeconds);
    static void enableAnsiOnWindows();
};
