#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif
#include "models/Customer.h"
#include "models/Restaurant.h"
#include "models/Cart.h"
#include "models/Order.h"
#include "models/Review.h"
#include "services/AuthService.h"
#include "services/RouteManager.h"
#include "services/RestaurantManager.h"
#include "services/DeliveryService.h"
#include "services/OrderManager.h"
#include "utils/InputValidator.h"
#include "utils/Display.h"
#include "utils/FoodRushException.h"

// ─────────────────────────────────────────────────────────────
//  Main Menu Loop — the core interactive session for a logged-in user
// ─────────────────────────────────────────────────────────────
void mainMenuLoop(std::unique_ptr<Customer>& customer, AuthService& auth,
                  RestaurantManager& restaurantMgr, OrderManager& orderMgr,
                  RouteManager& campusMap, DeliveryService& deliveryService) {

    Cart cart;

    while (true) {
        Display::clearScreen();
        std::cout << "\n";
        Display::showDivider();
        std::cout << "     \xF0\x9F\x93\x8B Main Menu \xE2\x80\x94 Hi, " << customer->getUsername() << "!\n";
        std::cout << "     \xF0\x9F\x92\xB0 Wallet: \xE2\x82\xB9" << customer->getWalletBalance() << "\n";
        Display::showDivider();
        std::cout << "   1.  \xF0\x9F\x8D\xBD\xEF\xB8\x8F  Browse Restaurants\n";
        std::cout << "   2.  \xF0\x9F\x94\x8D Search Food Items\n";
        std::cout << "   3.  \xF0\x9F\x9B\x92 View Cart\n";
        std::cout << "   4.  \xF0\x9F\x92\xB3 Checkout\n";
        std::cout << "   5.  \xF0\x9F\x93\xA6 Order History\n";
        std::cout << "   6.  \xF0\x9F\x9A\x97 Track Order\n";
        std::cout << "   7.  \xE2\xAD\x90 Rate a Restaurant\n";
        std::cout << "   8.  \xF0\x9F\x92\xB0 Add Funds to Wallet\n";
        std::cout << "   9.  \xF0\x9F\x91\xA4 My Dashboard\n";
        std::cout << "   10. \xE2\x9D\x8C Cancel Order\n";
        std::cout << "   11. \xF0\x9F\x9A\xAA Logout\n";
        Display::showDivider();

        int choice = InputValidator::getValidInt("  Enter choice", 1, 11);

        switch (choice) {

        // ── 1. Browse Restaurants ────────────────────────────
        case 1: {
            Display::clearScreen();
            Display::showHeader("Browse Restaurants");
            restaurantMgr.displayAllRestaurants();

            int restId = InputValidator::getValidInt(
                "\n  Select restaurant ID (0 to go back)", 0, restaurantMgr.getRestaurantCount());
            if (restId == 0) break;

            Restaurant* rest = restaurantMgr.getRestaurant(restId);
            if (!rest) { Display::showError("Restaurant not found."); Display::pressEnterToContinue(); break; }

            bool browsing = true;
            while (browsing) {
                Display::clearScreen();
                Display::showSubHeader(rest->getName() + " \xE2\x80\x94 Menu");
                rest->displayMenu();

                std::cout << "\n  1. \xF0\x9F\x9B\x92 Add item to cart\n";
                std::cout << "  2. \xF0\x9F\xA5\xA6 Filter Veg\n";
                std::cout << "  3. \xF0\x9F\x92\xB2 Sort by Price\n";
                std::cout << "  4. \xE2\xAC\x85 Back\n";

                int sub = InputValidator::getValidInt("  Choice", 1, 4);

                if (sub == 1) {
                    auto& menu = rest->getMenu();
                    if (menu.empty()) {
                        Display::showError("Menu is empty!");
                        Display::pressEnterToContinue();
                        continue;
                    }
                    int itemNum = InputValidator::getValidInt(
                        "  Enter item number", 1, static_cast<int>(menu.size()));
                    FoodItem selectedItem = menu[itemNum - 1];
                    try {
                        cart.addItem(selectedItem, rest->getName());
                    } catch (const ValidationException& e) {
                        Display::showError(e.what());
                    }
                    Display::pressEnterToContinue();
                }
                else if (sub == 2) {
                    Display::clearScreen();
                    Display::showSubHeader("Vegetarian Items \xE2\x80\x94 " + rest->getName());
                    auto vegItems = rest->filterVeg();
                    if (vegItems.empty()) {
                        Display::showInfo("No vegetarian items found.");
                    } else {
                        int idx = 1;
                        for (auto& item : vegItems) {
                            std::cout << "  " << idx++ << ". " << item.getName()
                                      << "  \xE2\x82\xB9" << item.getPrice()
                                      << (item.getIsVeg() ? "  [VEG]" : "  [NON-VEG]") << "\n";
                        }
                    }
                    Display::pressEnterToContinue();
                }
                else if (sub == 3) {
                    int order = InputValidator::getValidInt(
                        "  1. Ascending  2. Descending \xE2\x80\x94 Choice", 1, 2);
                    auto sortedMenu = rest->sortMenuByPrice(order == 1);
                    Display::clearScreen();
                    Display::showSubHeader(rest->getName() + " \xE2\x80\x94 Sorted by Price");
                    int idx = 1;
                    for (auto& item : sortedMenu) {
                        std::cout << "   " << std::left << std::setw(4) << idx++
                                  << item << "\n";
                    }
                    Display::pressEnterToContinue();
                }
                else {
                    browsing = false;
                }
            }
            break;
        }

        // ── 2. Search Food Items ─────────────────────────────
        case 2: {
            Display::clearScreen();
            Display::showHeader("Search Food Items");
            std::string keyword = InputValidator::getValidString("  Enter search keyword", 1);
            auto results = restaurantMgr.searchByFoodItem(keyword);

            if (results.empty()) {
                Display::showInfo("No items found matching \"" + keyword + "\".");
                Display::pressEnterToContinue();
                break;
            }

            Display::showSuccess("Found items in " + std::to_string(results.size()) + " restaurant(s):\n");

            // Collect all matching items for add-to-cart
            struct SearchResult {
                FoodItem item;
                std::string restaurantName;
            };
            std::vector<SearchResult> allResults;

            for (auto* r : results) {
                Display::showSubHeader(r->getName());
                auto matches = r->searchMenu(keyword);
                for (auto& item : matches) {
                    allResults.push_back({item, r->getName()});
                    std::cout << "    " << allResults.size() << ". "
                              << item.getName() << "  \xE2\x82\xB9" << item.getPrice()
                              << (item.getIsVeg() ? "  [VEG]" : "  [NON-VEG]") << "\n";
                }
            }

            std::cout << "\n";
            if (InputValidator::confirmAction("  Add an item to cart?")) {
                int pick = InputValidator::getValidInt(
                    "  Enter item number", 1, static_cast<int>(allResults.size()));
                auto& sr = allResults[pick - 1];
                try {
                    cart.addItem(sr.item, sr.restaurantName);
                } catch (const ValidationException& e) {
                    Display::showError(e.what());
                }
            }
            Display::pressEnterToContinue();
            break;
        }

        // ── 3. View Cart (with quantity controls) ────────────
        case 3: {
            Display::clearScreen();
            Display::showHeader("Your Cart");
            cart.displayCart();

            if (!cart.isEmpty()) {
                std::cout << "\n  1. \xE2\x9E\x95 Increment item qty\n";
                std::cout << "  2. \xE2\x9E\x96 Decrement item qty\n";
                std::cout << "  3. \xE2\x9D\x8C Remove item entirely\n";
                std::cout << "  4. \xF0\x9F\x97\x91\xEF\xB8\x8F  Clear cart\n";
                std::cout << "  5. \xE2\xAC\x85 Back\n";
                int sub = InputValidator::getValidInt("  Choice", 1, 5);

                if (sub == 1 || sub == 2 || sub == 3) {
                    // Build a numbered list of items for selection
                    auto& cartItems = cart.getItems();
                    std::vector<FoodItem> itemList;
                    for (auto it = cartItems.begin(); it != cartItems.end(); ++it) {
                        itemList.push_back(it->first);
                    }

                    int pick = InputValidator::getValidInt(
                        "  Enter item number", 1, static_cast<int>(itemList.size()));
                    const FoodItem& selected = itemList[pick - 1];

                    try {
                        if (sub == 1) {
                            cart.incrementItem(selected);
                        } else if (sub == 2) {
                            cart.decrementItem(selected);
                        } else {
                            cart.removeItem(selected);
                        }
                    } catch (const ValidationException& e) {
                        Display::showError(e.what());
                    }
                } else if (sub == 4) {
                    cart.clearCart();
                    Display::showSuccess("Cart cleared!");
                }
            }
            Display::pressEnterToContinue();
            break;
        }

        // ── 4. Checkout (with real-time delivery simulation) ─
        case 4: {
            Display::clearScreen();
            Display::showHeader("Checkout");

            if (cart.isEmpty()) {
                Display::showError("Your cart is empty! Add some items first.");
                Display::pressEnterToContinue();
                break;
            }

            cart.displayCart();

            // Delivery location
            std::cout << "\n  \xF0\x9F\x93\x8D Select your delivery location:\n";
            std::cout << "    1. Men's Hostel\n";
            std::cout << "    2. PRP Block\n";
            std::cout << "    3. Main Gate\n";
            std::cout << "    4. Library Square\n";
            std::cout << "    5. Tech Park\n";
            std::cout << "    6. Sports Complex\n";

            int locChoice = InputValidator::getValidInt("  Location", 1, 6);
            std::string userLocation;
            switch (locChoice) {
                case 1: userLocation = "Men's Hostel";    break;
                case 2: userLocation = "PRP Block";       break;
                case 3: userLocation = "Main Gate";       break;
                case 4: userLocation = "Library Square";  break;
                case 5: userLocation = "Tech Park";       break;
                case 6: userLocation = "Sports Complex";  break;
            }

            // Find restaurant location
            std::string currentRest = cart.getCurrentRestaurant();
            std::string restaurantLocation;
            for (int i = 1; i <= restaurantMgr.getRestaurantCount(); ++i) {
                Restaurant* r = restaurantMgr.getRestaurant(i);
                if (r && r->getName() == currentRest) {
                    restaurantLocation = r->getLocation();
                    break;
                }
            }

            try {
                // Show route preview
                Display::showSubHeader("Delivery Route");
                campusMap.displayRoute(restaurantLocation, userLocation);

                double distance = campusMap.findShortestDistance(restaurantLocation, userLocation);
                double estTime  = campusMap.estimateDeliveryTime(distance);

                // Order cost preview
                double subtotal    = cart.calculateSubtotal();
                double tax         = subtotal * 0.05;
                double deliveryFee = 10.0 + (distance * 5.0);
                double total       = subtotal + tax + deliveryFee;

                Display::showSubHeader("Order Summary");
                std::cout << "    Subtotal      : \xE2\x82\xB9" << subtotal << "\n";
                std::cout << "    Tax (5%)      : \xE2\x82\xB9" << tax << "\n";
                std::cout << "    Delivery Fee  : \xE2\x82\xB9" << deliveryFee << "\n";
                Display::showDivider();
                std::cout << "    \xF0\x9F\x92\xB5 TOTAL     : \xE2\x82\xB9" << total << "\n";
                std::cout << "    \xF0\x9F\x93\x8F Distance  : " << distance << " km\n";
                std::cout << "    \xE2\x8F\xB1 Est. Time : " << estTime << " min\n\n";

                if (customer->getWalletBalance() < total) {
                    Display::showError("Insufficient wallet balance! You need \xE2\x82\xB9" +
                                       std::to_string(total) + " but have \xE2\x82\xB9" +
                                       std::to_string(customer->getWalletBalance()));
                    Display::pressEnterToContinue();
                    break;
                }

                if (!InputValidator::confirmAction("  Confirm order?")) {
                    Display::showInfo("Order cancelled.");
                    Display::pressEnterToContinue();
                    break;
                }

                // Place order
                Order* order = orderMgr.placeOrder(
                    *customer, cart, campusMap, deliveryService,
                    auth, userLocation, restaurantLocation, currentRest);

                if (order) {
                    Display::showSuccess("Order placed successfully!");
                    std::cout << "\n";

                    // ⏱️ Real-Time Delivery Simulation with progress bars
                    order->setStatus(OrderStatus::PREPARING);
                    Display::showProgressBar("\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x8D\xB3 Preparing your order...", 3);
                    Display::showSuccess("Food is ready!");

                    order->setStatus(OrderStatus::OUT_FOR_DELIVERY);
                    Display::showProgressBar("\xF0\x9F\x9A\xB4 Out for delivery...", 4);
                    Display::showInfo("Almost there!");

                    Display::showProgressBar("\xF0\x9F\x93\x8D Arriving at your location...", 2);
                    order->setStatus(OrderStatus::DELIVERED);
                    Display::showSuccess("\xE2\x9C\x85 Order delivered! Enjoy your meal! \xF0\x9F\x8E\x89");

                    order->displaySummary();
                }
            } catch (const RouteException& e) {
                Display::showError(e.what());
            } catch (const OrderException& e) {
                Display::showError(e.what());
            } catch (const FoodRushException& e) {
                Display::showError(e.what());
            }
            Display::pressEnterToContinue();
            break;
        }

        // ── 5. Order History ─────────────────────────────────
        case 5: {
            Display::clearScreen();
            Display::showHeader("Order History");
            auto orders = orderMgr.getOrderHistory(customer->getUsername());

            if (orders.empty()) {
                Display::showInfo("No orders yet. Start ordering!");
                Display::pressEnterToContinue();
                break;
            }

            for (auto& o : orders) {
                o.displaySummary();
                std::cout << "\n";
            }

            if (InputValidator::confirmAction("  View details of a specific order?")) {
                int oid = InputValidator::getValidInt("  Enter Order ID", 1, 99999);
                Order* detail = orderMgr.getOrder(oid);
                if (detail) {
                    detail->displayDetailed();
                } else {
                    Display::showError("Order not found.");
                }
            }
            Display::pressEnterToContinue();
            break;
        }

        // ── 6. Track Order ───────────────────────────────────
        case 6: {
            Display::clearScreen();
            Display::showHeader("Track Order");
            int oid = InputValidator::getValidInt("  Enter Order ID", 1, 99999);
            orderMgr.trackOrder(oid);
            Display::pressEnterToContinue();
            break;
        }

        // ── 7. Rate a Restaurant ─────────────────────────────
        case 7: {
            Display::clearScreen();
            Display::showHeader("Rate a Restaurant");
            restaurantMgr.displayAllRestaurants();

            int restId = InputValidator::getValidInt(
                "\n  Select restaurant to rate (1-" + std::to_string(restaurantMgr.getRestaurantCount()) + ")",
                1, restaurantMgr.getRestaurantCount());
            Restaurant* rest = restaurantMgr.getRestaurant(restId);
            if (!rest) {
                Display::showError("Restaurant not found.");
                Display::pressEnterToContinue();
                break;
            }

            int rating = InputValidator::getValidInt(
                "  Rating (1-5 stars)", 1, 5);
            std::string comment = InputValidator::getValidString(
                "  Write your review", 3);

            Review review(customer->getUsername(), rest->getName(), rating, comment);
            orderMgr.saveReview(review);
            rest->addReview(review);

            Display::showSuccess("Thank you for your review! \xE2\xAD\x90");
            Display::pressEnterToContinue();
            break;
        }

        // ── 8. Add Funds ─────────────────────────────────────
        case 8: {
            Display::clearScreen();
            Display::showHeader("Add Funds to Wallet");
            std::cout << "  Current balance: \xE2\x82\xB9" << customer->getWalletBalance() << "\n\n";

            double amount = InputValidator::getValidDouble(
                "  Enter amount to add", 1.0, 100000.0);

            customer->addFunds(amount);
            try {
                auth.updateWalletBalance(customer->getUsername(), customer->getWalletBalance());
            } catch (const AuthException& e) {
                Display::showWarning(e.what());
            }

            Display::showSuccess("Added \xE2\x82\xB9" + std::to_string(amount) +
                                 " | New balance: \xE2\x82\xB9" +
                                 std::to_string(customer->getWalletBalance()));
            Display::pressEnterToContinue();
            break;
        }

        // ── 9. My Dashboard ──────────────────────────────────
        case 9: {
            Display::clearScreen();
            Display::showHeader("My Dashboard");
            customer->displayDashboard();

            auto orders = orderMgr.getOrderHistory(customer->getUsername());
            double totalSpent = 0;
            for (auto& o : orders) totalSpent += o.getTotal();

            std::cout << "\n  \xF0\x9F\x93\x8A Total Orders : " << orders.size() << "\n";
            std::cout << "  \xF0\x9F\x92\xB8 Total Spent  : \xE2\x82\xB9" << totalSpent << "\n";
            Display::pressEnterToContinue();
            break;
        }

        // ── 10. Cancel Order ─────────────────────────────────
        case 10: {
            Display::clearScreen();
            Display::showHeader("Cancel Order");

            auto orders = orderMgr.getOrderHistory(customer->getUsername());
            if (orders.empty()) {
                Display::showInfo("No orders to cancel.");
                Display::pressEnterToContinue();
                break;
            }

            // Show cancellable orders
            Display::showSubHeader("Your Orders");
            for (auto& o : orders) {
                o.displaySummary();
            }

            int oid = InputValidator::getValidInt("\n  Enter Order ID to cancel (0 to go back)", 0, 99999);
            if (oid == 0) break;

            try {
                orderMgr.cancelOrder(oid, *customer, auth, deliveryService);
                Display::showSuccess("Order #" + std::to_string(oid) + " cancelled! Refund added to wallet.");
                std::cout << "  \xF0\x9F\x92\xB0 New wallet balance: \xE2\x82\xB9"
                          << customer->getWalletBalance() << "\n";
            } catch (const OrderException& e) {
                Display::showError(e.what());
            } catch (const FoodRushException& e) {
                Display::showError(e.what());
            }
            Display::pressEnterToContinue();
            break;
        }

        // ── 11. Logout ───────────────────────────────────────
        case 11: {
            try {
                auth.updateWalletBalance(customer->getUsername(), customer->getWalletBalance());
            } catch (const AuthException& e) {
                Display::showWarning(e.what());
            }
            Display::showSuccess("Wallet saved. Goodbye, " + customer->getUsername() + "! \xF0\x9F\x91\x8B");
            Display::pressEnterToContinue();
            return;
        }

        default:
            Display::showError("Invalid choice.");
            Display::pressEnterToContinue();
            break;
        }
    }
}

// ─────────────────────────────────────────────────────────────
//  main() — Application entry point
// ─────────────────────────────────────────────────────────────
int main() {
    // Enable ANSI color codes on Windows terminal
    Display::enableAnsiOnWindows();

    // 1. Ensure data directory exists
    MKDIR("data");

    // 2. Initialize services
    RouteManager campusMap;
    RestaurantManager restaurantMgr;   // Loads from data/restaurants.csv (or defaults)
    DeliveryService deliveryService;
    AuthService auth;
    OrderManager orderMgr;

    // 3. Load campus road network from file (with hardcoded fallback)
    campusMap.loadFromFile("data/campus_map.csv");
    if (!campusMap.hasLocations()) {
        // Fallback: hardcoded campus road network
        campusMap.addRoad("Driver Hub", "Main Gate", 1.0);
        campusMap.addRoad("Main Gate", "Food Court", 1.5);
        campusMap.addRoad("Main Gate", "Library Square", 2.0);
        campusMap.addRoad("Food Court", "PRP Block", 0.8);
        campusMap.addRoad("Food Court", "Men's Hostel", 2.5);
        campusMap.addRoad("PRP Block", "Men's Hostel", 3.0);
        campusMap.addRoad("PRP Block", "Tech Park", 1.2);
        campusMap.addRoad("Library Square", "Tech Park", 0.5);
        campusMap.addRoad("Tech Park", "Sports Complex", 1.8);
        campusMap.addRoad("Sports Complex", "Men's Hostel", 2.0);
        campusMap.addRoad("Library Square", "Sports Complex", 1.5);
    }

    // 4. Load persistent data
    orderMgr.loadOrdersFromFile();

    // Load saved reviews and attach them to restaurants
    auto allReviews = orderMgr.loadAllReviews();
    for (auto& review : allReviews) {
        for (int i = 1; i <= restaurantMgr.getRestaurantCount(); ++i) {
            Restaurant* r = restaurantMgr.getRestaurant(i);
            if (r && r->getName() == review.getRestaurantName()) {
                r->addReview(review);
                break;
            }
        }
    }

    // ── Welcome Screen Loop ────────────────────────────
    while (true) {
        Display::clearScreen();
        Display::showWelcomeBanner();

        std::cout << "\n";
        std::cout << "  1. \xF0\x9F\x93\x9D Register\n";
        std::cout << "  2. \xF0\x9F\x94\x91 Login\n";
        std::cout << "  3. \xF0\x9F\x9A\xAA Exit\n\n";

        int choice = InputValidator::getValidInt("  Enter choice", 1, 3);

        switch (choice) {

        // ── Register ───────────────────────────
        case 1: {
            Display::clearScreen();
            Display::showHeader("Register New Account");

            std::string username = InputValidator::getValidString("  Username", 3);
            std::string password = InputValidator::getValidString("  Password (min 4 chars)", 4);

            try {
                auth.registerCustomer(username, password);
                Display::showSuccess("Account created successfully! You can now login. \xF0\x9F\x8E\x89");
            } catch (const AuthException& e) {
                Display::showError(e.what());
            }
            Display::pressEnterToContinue();
            break;
        }

        // ── Login ──────────────────────────────
        case 2: {
            Display::clearScreen();
            Display::showHeader("Login");

            std::string username = InputValidator::getValidString("  Username", 1);
            std::string password = InputValidator::getValidString("  Password", 1);

            try {
                auto customer = auth.loginCustomer(username, password);
                Display::showSuccess("Welcome back, " + username + "! \xF0\x9F\x91\x8B");
                Display::pressEnterToContinue();
                mainMenuLoop(customer, auth, restaurantMgr, orderMgr,
                             campusMap, deliveryService);
            } catch (const AuthException& e) {
                Display::showError(e.what());
                Display::pressEnterToContinue();
            }
            break;
        }

        // ── Exit ───────────────────────────────
        case 3: {
            Display::clearScreen();
            std::cout << "\n";
            Display::showDivider();
            std::cout << "  \xF0\x9F\x91\x8B  Thanks for using FoodRush!\n";
            std::cout << "     Craving something? Come back anytime! \xF0\x9F\x8D\x95\n";
            Display::showDivider();
            std::cout << "\n";
            return 0;
        }

        default:
            Display::showError("Invalid choice.");
            Display::pressEnterToContinue();
            break;
        }
    }

    return 0;
}