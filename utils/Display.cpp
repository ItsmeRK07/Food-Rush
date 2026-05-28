#include "Display.h"
#include <limits>
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <unistd.h>
#endif

// ============================================================================
// Display Implementation
// OOP Concept: Abstraction - centralizes all UI formatting decisions.
// Changing a divider width or color scheme only requires edits here.
// ============================================================================

void Display::enableAnsiOnWindows() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    SetConsoleOutputCP(CP_UTF8);
#endif
}

// Platform-agnostic sleep helper (milliseconds)
static void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void Display::showHeader(const std::string& title) {
    std::cout << "\r\n  " << CYAN << BOLD << std::string(50, '=') << RESET << "\r\n";
    std::cout << "   " << CYAN << BOLD << title << RESET << "\r\n";
    std::cout << "  " << CYAN << BOLD << std::string(50, '=') << RESET << "\r\n";
}

void Display::showSubHeader(const std::string& title) {
    std::cout << "\r\n  " << CYAN << std::string(40, '-') << RESET << "\r\n";
    std::cout << "   " << CYAN << title << RESET << "\r\n";
    std::cout << "  " << CYAN << std::string(40, '-') << RESET << "\r\n";
}

void Display::showDivider() {
    std::cout << "  " << DIM << std::string(50, '-') << RESET << "\r\n";
}

void Display::showSuccess(const std::string& message) {
    std::cout << "  " << GREEN << BOLD << "\xE2\x9C\x85 " << message << RESET << "\r\n";
}

void Display::showError(const std::string& message) {
    std::cout << "  " << RED << BOLD << "\xE2\x9D\x8C " << message << RESET << "\r\n";
}

void Display::showWarning(const std::string& message) {
    std::cout << "  " << YELLOW << BOLD << "\xE2\x9A\xA0\xEF\xB8\x8F  " << message << RESET << "\r\n";
}

void Display::showInfo(const std::string& message) {
    std::cout << "  " << CYAN << "\xE2\x84\xB9\xEF\xB8\x8F  " << message << RESET << "\r\n";
}

void Display::pressEnterToContinue() {
    std::cout << "\r\n  " << DIM << "Press Enter to continue..." << RESET;
    std::string dummy;
    std::getline(std::cin, dummy);
}

void Display::clearScreen() {
    system("cls");
}

void Display::showWelcomeBanner() {
    std::cout << "\r\n";
    std::cout << "  " << GREEN << BOLD;
    std::cout << "\xE2\x95\x94\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x97\r\n";
    std::cout << "  \xE2\x95\x91                                                  \xE2\x95\x91\r\n";
    std::cout << "  \xE2\x95\x91      \xF0\x9F\x8D\x95  Welcome to FoodRush!  \xF0\x9F\x8D\x94              \xE2\x95\x91\r\n";
    std::cout << "  \xE2\x95\x91        Fast Food, Faster Delivery! \xF0\x9F\x9A\x80            \xE2\x95\x91\r\n";
    std::cout << "  \xE2\x95\x91                                                  \xE2\x95\x91\r\n";
    std::cout << "  \xE2\x95\x9A\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x9D\r\n";
    std::cout << RESET;
    std::cout << "\r\n";
}

void Display::showProgressBar(const std::string& label, int durationSeconds) {
    const int barWidth = 30;
    const int steps = 20;
    int sleepTime = (durationSeconds * 1000) / steps;

    std::cout << "\r\n";
    for (int i = 0; i <= steps; ++i) {
        int filled = (i * barWidth) / steps;
        int empty = barWidth - filled;
        int percent = (i * 100) / steps;

        std::cout << "\r  " << CYAN << label << "  " << GREEN << BOLD << "[";
        for (int f = 0; f < filled; ++f) std::cout << "\xE2\x96\x88";
        std::cout << RESET << DIM;
        for (int e = 0; e < empty; ++e) std::cout << "\xE2\x96\x91";
        std::cout << RESET << GREEN << BOLD << "] " << percent << "%" << RESET;
        std::cout.flush();

        if (i < steps) {
            sleepMs(sleepTime);
        }
    }
    std::cout << "\r\n";
}
