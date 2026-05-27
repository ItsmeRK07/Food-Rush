#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "FoodItem.h" // Including the FoodItem class

class Restaurant {
private:
    std::string name;
    std::vector<FoodItem> menu; 

public:
    Restaurant(std::string n) : name(n) {}

    void addFoodItem(FoodItem item) {
        menu.push_back(item);
    }

    void displayMenu() const {
        std::cout << "--- " << name << " Menu ---\n";
        for (const auto& item : menu) {
            std::cout << (item.getIsVeg() ? "[V] " : "[NV] ") 
                      << item.getName() << " : $" << item.getPrice() << "\n";
        }
    }
};