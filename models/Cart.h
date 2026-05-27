#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "FoodItem.h"

class Cart {
private:
    std::vector<FoodItem> items;
    std::string currentRestaurant;

public:
    Cart() : currentRestaurant("") {}

    // Add item with Restaurant validation
    bool addItem(FoodItem item, std::string restaurantName) {
        if (items.empty()) {
            currentRestaurant = restaurantName; // Set the cart's restaurant lock
        } else if (currentRestaurant != restaurantName) {
            std::cout << "❌ Error: You can only order from one restaurant at a time. Clear cart to switch.\n";
            return false;
        }

        items.push_back(item);
        std::cout << "🛒 Added " << item.getName() << " to cart.\n";
        return true;
    }

    double calculateSubtotal() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getPrice();
        }
        return total;
    }

    void displayCart() const {
        if (items.empty()) {
            std::cout << "Your cart is empty.\n";
            return;
        }
        std::cout << "\n--- Your Cart (" << currentRestaurant << ") ---\n";
        for (const auto& item : items) {
            std::cout << "- " << item.getName() << " : $" << item.getPrice() << "\n";
        }
        std::cout << "Subtotal: $" << calculateSubtotal() << "\n";
    }

    void clearCart() {
        items.clear();
        currentRestaurant = "";
    }
    
    bool isEmpty() const { return items.empty(); }
};