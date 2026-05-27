#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "../models/Restaurant.h"

// Composition: Manages collection of Restaurant objects
// Encapsulation: Restaurant data initialization hidden in private method
class RestaurantManager {
private:
    std::vector<Restaurant> restaurants;
    void initializeRestaurants();

public:
    RestaurantManager();
    void displayAllRestaurants() const;
    Restaurant* getRestaurant(int id);
    const Restaurant* getRestaurant(int id) const;
    std::vector<Restaurant*> searchByFoodItem(const std::string& keyword);
    void displayRestaurantMenu(int restaurantId) const;
};
