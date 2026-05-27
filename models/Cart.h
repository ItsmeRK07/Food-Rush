#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "FoodItem.h"

// ============================================================================
// Cart - Shopping Cart Model
// OOP Concept: Composition - contains a vector of FoodItems representing
//              the items the customer wants to order.
// OOP Concept: Encapsulation - addItem validates that items come from only
//              one restaurant at a time, enforcing a business rule internally.
// ============================================================================
class Cart {
private:
    std::vector<FoodItem> items;
    std::string currentRestaurant;

public:
    Cart();

    // Cart operations with built-in validation (Encapsulation)
    bool addItem(const FoodItem& item, const std::string& restaurantName);
    bool removeItem(int index);
    double calculateSubtotal() const;
    void displayCart() const;
    void clearCart();
    bool isEmpty() const;

    // Getters - Encapsulation
    const std::vector<FoodItem>& getItems() const;
    std::string getCurrentRestaurant() const;

    // Operator Overloading: formatted stream output
    friend std::ostream& operator<<(std::ostream& os, const Cart& cart);
};