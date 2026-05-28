#include "RestaurantManager.h"
#include <iomanip>

// Constructor: Loads restaurants from CSV file, with hardcoded fallback
// Composition: RestaurantManager "has-a" collection of Restaurant objects
RestaurantManager::RestaurantManager() {
    loadFromFile("data/restaurants.csv");
    if (restaurants.empty()) {
        loadDefaults();
    }
}

// Loads restaurant and menu data from a CSV file
// Format: RESTAURANT,id,name,location   and   ITEM,restaurant_id,item_id,name,price,is_veg
void RestaurantManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return; // File not found — caller will use loadDefaults()
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string type;
        std::getline(iss, type, ',');

        if (type == "RESTAURANT") {
            std::string idStr, name, location;
            std::getline(iss, idStr, ',');
            std::getline(iss, name, ',');
            std::getline(iss, location);

            try {
                int id = std::stoi(idStr);
                restaurants.push_back(Restaurant(id, name, location));
            } catch (...) {
                continue; // Skip malformed restaurant line
            }
        }
        else if (type == "ITEM") {
            std::string restIdStr, itemIdStr, name, priceStr, vegStr;
            std::getline(iss, restIdStr, ',');
            std::getline(iss, itemIdStr, ',');
            std::getline(iss, name, ',');
            std::getline(iss, priceStr, ',');
            std::getline(iss, vegStr);

            try {
                int restId = std::stoi(restIdStr);
                int itemId = std::stoi(itemIdStr);
                double price = std::stod(priceStr);
                bool isVeg = (vegStr == "1");

                // Find the restaurant and add the item
                for (auto& r : restaurants) {
                    if (r.getId() == restId) {
                        r.addFoodItem(FoodItem(itemId, name, price, isVeg));
                        break;
                    }
                }
            } catch (...) {
                continue; // Skip malformed item line
            }
        }
    }

    file.close();
}

// Encapsulation: Hardcoded fallback data — used when CSV file is missing
void RestaurantManager::loadDefaults() {
    // ─── Restaurant 1: Campus Kitchen ───────────────────────
    Restaurant r1(1, "Campus Kitchen", "Food Court");
    r1.addFoodItem(FoodItem(1, "Paneer Lababdar", 180.00, true));
    r1.addFoodItem(FoodItem(2, "Butter Chicken", 220.00, false));
    r1.addFoodItem(FoodItem(3, "Dal Makhani", 150.00, true));
    r1.addFoodItem(FoodItem(4, "Garlic Naan", 40.00, true));
    r1.addFoodItem(FoodItem(5, "Jeera Rice", 80.00, true));
    restaurants.push_back(r1);

    // ─── Restaurant 2: Dragon Wok ───────────────────────────
    Restaurant r2(2, "Dragon Wok", "Tech Park");
    r2.addFoodItem(FoodItem(6, "Veg Hakka Noodles", 120.00, true));
    r2.addFoodItem(FoodItem(7, "Chicken Fried Rice", 160.00, false));
    r2.addFoodItem(FoodItem(8, "Veg Manchurian", 140.00, true));
    r2.addFoodItem(FoodItem(9, "Spring Rolls", 100.00, true));
    r2.addFoodItem(FoodItem(10, "Chilli Chicken", 180.00, false));
    restaurants.push_back(r2);

    // ─── Restaurant 3: South Express ────────────────────────
    Restaurant r3(3, "South Express", "Library Square");
    r3.addFoodItem(FoodItem(11, "Masala Dosa", 90.00, true));
    r3.addFoodItem(FoodItem(12, "Idli Sambar", 60.00, true));
    r3.addFoodItem(FoodItem(13, "Medu Vada", 70.00, true));
    r3.addFoodItem(FoodItem(14, "Uttapam", 100.00, true));
    r3.addFoodItem(FoodItem(15, "Filter Coffee", 30.00, true));
    restaurants.push_back(r3);

    // ─── Restaurant 4: Burger Junction ──────────────────────
    Restaurant r4(4, "Burger Junction", "Sports Complex");
    r4.addFoodItem(FoodItem(16, "Classic Veg Burger", 110.00, true));
    r4.addFoodItem(FoodItem(17, "Chicken Burger", 140.00, false));
    r4.addFoodItem(FoodItem(18, "French Fries", 80.00, true));
    r4.addFoodItem(FoodItem(19, "Chocolate Shake", 90.00, true));
    r4.addFoodItem(FoodItem(20, "Chicken Wrap", 130.00, false));
    restaurants.push_back(r4);

    // ─── Restaurant 5: Chai & More ──────────────────────────
    Restaurant r5(5, "Chai & More", "Main Gate");
    r5.addFoodItem(FoodItem(21, "Masala Chai", 20.00, true));
    r5.addFoodItem(FoodItem(22, "Samosa", 15.00, true));
    r5.addFoodItem(FoodItem(23, "Maggi", 40.00, true));
    r5.addFoodItem(FoodItem(24, "Veg Sandwich", 60.00, true));
    r5.addFoodItem(FoodItem(25, "Cold Coffee", 50.00, true));
    restaurants.push_back(r5);
}

// Displays all restaurants in a formatted list
// Uses operator<< or custom formatting for clean output
void RestaurantManager::displayAllRestaurants() const {
    std::cout << "\n";
    std::cout << "  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << std::endl;
    std::cout << "  \u2551       \U0001F37D  AVAILABLE RESTAURANTS          \u2551" << std::endl;
    std::cout << "  \u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D" << std::endl;
    std::cout << std::endl;

    for (const auto& restaurant : restaurants) {
        double avgRating = restaurant.getAverageRating();
        std::cout << "  [" << restaurant.getId() << "] "
                  << restaurant.getName()
                  << "  \U0001F4CD " << restaurant.getLocation();

        if (avgRating > 0) {
            std::cout << "  \u2B50 " << std::fixed << std::setprecision(1) << avgRating;
        } else {
            std::cout << "  \u2B50 New";
        }

        std::cout << "  (" << restaurant.getMenu().size() << " items)" << std::endl;
    }
    std::cout << std::endl;
}

// Returns a mutable pointer to a restaurant by ID
// Returns nullptr if not found (safe pointer pattern)
Restaurant* RestaurantManager::getRestaurant(int id) {
    for (auto& restaurant : restaurants) {
        if (restaurant.getId() == id) {
            return &restaurant;
        }
    }
    return nullptr;
}

// Const overload: Returns a const pointer for read-only access
// Polymorphism: Const-correctness through method overloading
const Restaurant* RestaurantManager::getRestaurant(int id) const {
    for (const auto& restaurant : restaurants) {
        if (restaurant.getId() == id) {
            return &restaurant;
        }
    }
    return nullptr;
}

// Searches all restaurants for menu items matching a keyword
// Returns vector of pointers to restaurants with matching items
std::vector<Restaurant*> RestaurantManager::searchByFoodItem(const std::string& keyword) {
    std::vector<Restaurant*> results;

    for (auto& restaurant : restaurants) {
        // Delegation: Uses Restaurant's searchMenu method
        std::vector<FoodItem> matches = restaurant.searchMenu(keyword);
        if (!matches.empty()) {
            results.push_back(&restaurant);
        }
    }

    return results;
}

// Displays a specific restaurant's menu by ID
void RestaurantManager::displayRestaurantMenu(int restaurantId) const {
    const Restaurant* restaurant = getRestaurant(restaurantId);
    if (restaurant) {
        restaurant->displayMenu();
    } else {
        std::cout << "\n  \u274C Restaurant not found!" << std::endl;
    }
}

int RestaurantManager::getRestaurantCount() const {
    return static_cast<int>(restaurants.size());
}
