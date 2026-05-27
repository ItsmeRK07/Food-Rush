#pragma once
#include <string>

class FoodItem {
private:
    std::string name;
    double price;
    bool isVeg;

public:
    FoodItem(std::string n, double p, bool v) : name(n), price(p), isVeg(v) {}
    
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    bool getIsVeg() const { return isVeg; }
};