#include <iostream>
#include <string>
#include <memory>
#include "models/Customer.h"
#include "models/Restaurant.h"
#include "models/Cart.h"
#include "models/Order.h"
#include "services/AuthService.h"
#include "services/RouteManager.h"

int main() {
    RouteManager campusMap;
    campusMap.addRoad("Driver Hub", "Food Court", 1.5);
    campusMap.addRoad("Food Court", "PRP Block", 0.8);
    campusMap.addRoad("Food Court", "Men's Hostel", 2.5);
    campusMap.addRoad("PRP Block", "Men's Hostel", 3.0);

    Restaurant campusKitchen("Food Court Kitchen");
    campusKitchen.addFoodItem(FoodItem("Paneer Lababdar", 180.00, true));
    campusKitchen.addFoodItem(FoodItem("Veg Hakka Noodles", 120.00, true));
    campusKitchen.addFoodItem(FoodItem("Butter Paneer Kulcha", 90.00, true));

    AuthService auth;
    std::unique_ptr<Customer> currentUser = std::make_unique<Customer>("TestUser", "pass123");
    currentUser->addFunds(1000.0);

    std::cout << "\n";
    campusKitchen.displayMenu();

    Cart currentCart;
    currentCart.addItem(FoodItem("Paneer Lababdar", 180.00, true), "Food Court Kitchen");
    currentCart.addItem(FoodItem("Butter Paneer Kulcha", 90.00, true), "Food Court Kitchen");

    std::string userLoc = "Men's Hostel";
    std::string restLoc = "Food Court";

    Order orderProcessor;
    orderProcessor.placeOrder(*currentUser, currentCart, campusMap, userLoc, restLoc);

    return 0;
}