#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "FoodItem.h"
#include "Review.h"

// ============================================================================
// Restaurant - Composite Data Model
// OOP Concept: Composition - contains vectors of FoodItems and Reviews.
//              The restaurant "has-a" menu and "has-a" collection of reviews.
// OOP Concept: Encapsulation - rich interface for menu search, filtering,
//              and sorting without exposing internal data structures.
// ============================================================================
class Restaurant {
private:
    int id;
    std::string name;
    std::string location;
    std::vector<FoodItem> menu;
    std::vector<Review> reviews;

public:
    Restaurant(int id, const std::string& name, const std::string& location);

    // Getters - Encapsulation
    int getId() const;
    std::string getName() const;
    std::string getLocation() const;
    const std::vector<FoodItem>& getMenu() const;
    const std::vector<Review>& getReviews() const;

    // Composition: managing contained objects
    void addFoodItem(const FoodItem& item);
    void addReview(const Review& review);
    double getAverageRating() const;

    // Display
    void displayMenu() const;
    void displayReviews() const;

    // Search & Filter - Encapsulation of query logic
    std::vector<FoodItem> searchMenu(const std::string& keyword) const;
    std::vector<FoodItem> filterVeg() const;
    std::vector<FoodItem> sortMenuByPrice(bool ascending = true) const;

    // Operator Overloading: formatted stream output
    friend std::ostream& operator<<(std::ostream& os, const Restaurant& r);
};