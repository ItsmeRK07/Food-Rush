#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include "FoodItem.h"

// ============================================================================
// Cart - Shopping Cart Model (Quantity-Aware)
// OOP Concept: Composition - contains a map of FoodItems to quantities,
//              representing the items the customer wants to order.
// OOP Concept: Encapsulation - addItem validates that items come from only
//              one restaurant at a time, enforcing a business rule internally.
// DSA: std::map with custom comparator (FoodItem::operator<) for ordered
//      storage of items with O(log n) lookup, insert, and delete.
// ============================================================================
class Cart {
private:
    std::map<FoodItem, int> items;  // Maps FoodItem -> quantity
    std::string currentRestaurant;

public:
    Cart();

    // Cart operations with built-in validation (Encapsulation)
    bool addItem(const FoodItem& item, const std::string& restaurantName);
    bool removeItem(const FoodItem& item);
    void incrementItem(const FoodItem& item);
    void decrementItem(const FoodItem& item);
    double calculateSubtotal() const;
    void displayCart() const;
    void clearCart();
    bool isEmpty() const;
    int getItemCount() const;

    // Getters - Encapsulation
    const std::map<FoodItem, int>& getItems() const;
    std::string getCurrentRestaurant() const;

    // Returns expanded vector for Order construction (backward compatibility)
    std::vector<FoodItem> getItemsFlat() const;

    // Operator Overloading: formatted stream output
    friend std::ostream& operator<<(std::ostream& os, const Cart& cart);
};