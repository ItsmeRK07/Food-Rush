#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../models/Restaurant.h"

// Composition: Manages collection of Restaurant objects
// Encapsulation: Restaurant data initialization hidden in private method
class RestaurantManager {
private:
    std::vector<Restaurant> restaurants;
    void loadFromFile(const std::string& filename);
    void loadDefaults();

public:
    RestaurantManager();
    void displayAllRestaurants() const;
    Restaurant* getRestaurant(int id);
    const Restaurant* getRestaurant(int id) const;
    std::vector<Restaurant*> searchByFoodItem(const std::string& keyword);
    void displayRestaurantMenu(int restaurantId) const;
    int getRestaurantCount() const;
};
