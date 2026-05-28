#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>

// ============================================================================
// FoodItem - Core Data Model
// OOP Concept: Encapsulation - private data members with public getters
// OOP Concept: Operator Overloading - == for comparison, << for output
// ============================================================================
class FoodItem {
private:
    int id;
    std::string name;
    double price;
    bool isVeg;

public:
    // Constructors - default and parameterized
    FoodItem();
    FoodItem(int id, const std::string& name, double price, bool isVeg);

    // Getters - Encapsulation: controlled read-only access to private data
    int getId() const;
    std::string getName() const;
    double getPrice() const;
    bool getIsVeg() const;

    // Search functionality
    bool matchesSearch(const std::string& keyword) const;

    // Operator Overloading: equality comparison by id
    bool operator==(const FoodItem& other) const;

    // Operator Overloading: less-than comparison by id (for std::map key)
    bool operator<(const FoodItem& other) const;

    // Operator Overloading: stream insertion for formatted display
    friend std::ostream& operator<<(std::ostream& os, const FoodItem& item);
};