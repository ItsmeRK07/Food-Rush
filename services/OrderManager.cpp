#include "OrderManager.h"
#include <filesystem>
#include <algorithm>
#include <ctime>

// Constructor: Initialize file paths and pricing constants
OrderManager::OrderManager()
    : ordersFile("data/orders.txt"),
      reviewsFile("data/reviews.txt"),
      taxRate(0.05),
      deliveryFeePerKm(15.0) {
    // Ensure data directory exists
    std::filesystem::create_directories("data");
    // Load existing orders from persistent storage
    loadOrdersFromFile();
}

// Service-Oriented: Orchestrates the complete order placement workflow
// Coordinates Cart, RouteManager, DeliveryService, AuthService, and Customer
// Returns pointer to the placed order, or nullptr on failure
Order* OrderManager::placeOrder(Customer& customer, Cart& cart, RouteManager& routeManager,
                                 DeliveryService& deliveryService, AuthService& authService,
                                 const std::string& userLocation, const std::string& restaurantLocation,
                                 const std::string& restaurantName) {
    // Step 1: Validate cart is not empty
    if (cart.isEmpty()) {
        std::cout << "\n  \u274C Your cart is empty! Add items before placing an order." << std::endl;
        return nullptr;
    }

    // Step 2: Calculate route distance using Dijkstra's algorithm
    double distance = routeManager.findShortestDistance(restaurantLocation, userLocation);
    if (distance < 0) {
        std::cout << "\n  \u274C Delivery unavailable! No route from "
                  << restaurantLocation << " to " << userLocation << "." << std::endl;
        return nullptr;
    }

    // Step 3: Calculate order costs
    double subtotal = cart.calculateSubtotal();
    double tax = subtotal * taxRate;
    double deliveryFee = distance * deliveryFeePerKm;
    double total = subtotal + tax + deliveryFee;

    // Step 4: Estimate delivery time
    int estTime = routeManager.estimateDeliveryTime(distance);

    // Step 5: Assign a delivery partner
    DeliveryPartner* partner = deliveryService.assignPartner();
    if (partner == nullptr) {
        std::cout << "\n  \u274C No delivery partners available right now. Please try again later." << std::endl;
        return nullptr;
    }

    // Step 6: Check if customer has sufficient funds
    if (customer.getWalletBalance() < total) {
        std::cout << "\n  \u274C Insufficient funds! Required: \u20B9"
                  << std::fixed << std::setprecision(2) << total
                  << " | Available: \u20B9" << customer.getWalletBalance() << std::endl;
        return nullptr;
    }

    // Step 7: Deduct funds from customer wallet
    customer.deductFunds(total);
    authService.updateWalletBalance(customer.getUsername(), customer.getWalletBalance());

    // Step 8: Create the Order object
    // Composition: Order aggregates FoodItems, customer info, and delivery details
    Order order(customer.getUsername(), restaurantName, cart.getItems(),
                subtotal, tax, deliveryFee, total, distance, estTime);
    order.setStatus(OrderStatus::PLACED);

    // Step 9: Assign order to delivery partner
    std::string partnerName = partner->getUsername();
    partner->assignOrder(order.getOrderId());

    // Step 10: Record order in customer's history and master list
    customer.addOrder(order);
    allOrders.push_back(order);

    // Step 11: Persist to file
    saveOrdersToFile();

    // Step 12: Display checkout summary
    std::cout << "\n  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << std::endl;
    std::cout << "  \u2551       \U0001F9FE CHECKOUT SUMMARY            \u2551" << std::endl;
    std::cout << "  \u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Subtotal:      \u20B9" << subtotal << std::endl;
    std::cout << "  Tax (5%):      \u20B9" << tax << std::endl;
    std::cout << "  Distance:      " << std::setprecision(1) << distance << " km" << std::endl;
    std::cout << "  Delivery Fee:  \u20B9" << std::setprecision(2) << deliveryFee << std::endl;
    std::cout << "  \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500" << std::endl;
    std::cout << "  Total:         \u20B9" << total << std::endl;
    std::cout << std::endl;
    std::cout << "  \u2705 Order #" << order.getOrderId() << " Placed Successfully!" << std::endl;
    std::cout << "  \U0001F697 Delivery Partner: " << partnerName << std::endl;
    std::cout << "  \u23F1  Estimated Delivery: " << estTime << " minutes" << std::endl;

    // Step 13: Display delivery route
    routeManager.displayRoute(restaurantLocation, userLocation);

    std::cout << "\n  Remaining Balance: \u20B9" << customer.getWalletBalance() << std::endl;
    std::cout << std::endl;

    // Clear the cart after successful order
    cart.clearCart();

    // Return pointer to the order stored in allOrders
    return &allOrders.back();
}

// Filters all orders by customer name
// Returns copies of matching orders (value semantics for safety)
std::vector<Order> OrderManager::getOrderHistory(const std::string& customerName) const {
    std::vector<Order> history;
    for (const auto& order : allOrders) {
        if (order.getCustomerName() == customerName) {
            history.push_back(order);
        }
    }
    return history;
}

// Finds an order by ID and returns a mutable pointer
// Returns nullptr if not found
Order* OrderManager::getOrder(int orderId) {
    for (auto& order : allOrders) {
        if (order.getOrderId() == orderId) {
            return &order;
        }
    }
    return nullptr;
}

// Displays detailed tracking information for an order
void OrderManager::trackOrder(int orderId) const {
    for (const auto& order : allOrders) {
        if (order.getOrderId() == orderId) {
            order.displayDetailed();
            return;
        }
    }
    std::cout << "\n  \u274C Order #" << orderId << " not found!" << std::endl;
}

// Persists all orders to file in pipe-delimited format
// File Format: orderId|customerName|restaurantName|subtotal|tax|deliveryFee|total|distance|estTime|statusInt|partnerName|timestamp|items
// Items format: name:price,name:price,...
void OrderManager::saveOrdersToFile() const {
    std::ofstream file(ordersFile, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "\n  \u274C Error: Could not save orders to file!" << std::endl;
        return;
    }

    for (const auto& order : allOrders) {
        file << order.getOrderId() << "|"
             << order.getCustomerName() << "|"
             << order.getRestaurantName() << "|"
             << std::fixed << std::setprecision(2)
             << order.getSubtotal() << "|"
             << order.getTax() << "|"
             << order.getDeliveryFee() << "|"
             << order.getTotal() << "|"
             << order.getDistance() << "|"
             << order.getEstimatedTimeMinutes() << "|"
             << static_cast<int>(order.getStatus()) << "|"
             << order.getDeliveryPartnerName() << "|"
             << order.getTimestamp() << "|";

        // Serialize items as comma-separated name:price pairs
        const auto& items = order.getItems();
        for (size_t i = 0; i < items.size(); i++) {
            file << items[i].getName() << ":" << items[i].getPrice();
            if (i < items.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}

// Loads orders from persistent storage
// Reconstructs Order objects from pipe-delimited file format
void OrderManager::loadOrdersFromFile() {
    std::ifstream file(ordersFile);
    if (!file.is_open()) {
        return; // File doesn't exist yet — no orders to load
    }

    std::string line;
    int maxOrderId = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        // Parse pipe-delimited fields
        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 13) continue; // Malformed line

        try {
            int orderId = std::stoi(tokens[0]);
            std::string customerName = tokens[1];
            std::string restName = tokens[2];
            double subtotal = std::stod(tokens[3]);
            double tax = std::stod(tokens[4]);
            double deliveryFee = std::stod(tokens[5]);
            double total = std::stod(tokens[6]);
            double distance = std::stod(tokens[7]);
            int estTime = std::stoi(tokens[8]);
            int statusInt = std::stoi(tokens[9]);
            std::string partnerName = tokens[10];
            std::string timestamp = tokens[11];

            // Parse items from comma-separated name:price pairs
            std::vector<FoodItem> items;
            if (!tokens[12].empty()) {
                std::istringstream itemStream(tokens[12]);
                std::string itemToken;
                while (std::getline(itemStream, itemToken, ',')) {
                    size_t colonPos = itemToken.rfind(':');
                    if (colonPos != std::string::npos) {
                        std::string itemName = itemToken.substr(0, colonPos);
                        double itemPrice = std::stod(itemToken.substr(colonPos + 1));
                        items.push_back(FoodItem(0, itemName, itemPrice, true));
                    }
                }
            }

            // Reconstruct Order object using parameterized constructor
            Order order(customerName, restName, items, subtotal, tax, deliveryFee, total, distance, estTime);
            order.setOrderId(orderId);
            order.setStatus(static_cast<OrderStatus>(statusInt));

            allOrders.push_back(order);

            // Track maximum order ID for next order assignment
            if (orderId > maxOrderId) {
                maxOrderId = orderId;
            }
        } catch (const std::exception& e) {
            // Skip malformed lines gracefully
            continue;
        }
    }

    file.close();
}

// Saves a review to the reviews file in pipe-delimited format
// File Format: reviewerName|restaurantName|rating|comment|timestamp
void OrderManager::saveReview(const Review& review) const {
    // Ensure data directory exists
    std::filesystem::create_directories("data");

    std::ofstream file(reviewsFile, std::ios::app);
    if (!file.is_open()) {
        std::cout << "\n  \u274C Error: Could not save review!" << std::endl;
        return;
    }

    file << review.getReviewerName() << "|"
         << review.getRestaurantName() << "|"
         << review.getRating() << "|"
         << review.getComment() << "|"
         << review.getTimestamp() << "\n";

    file.close();
}

// Loads reviews for a specific restaurant from file
// Returns vector of Review objects filtered by restaurant name
std::vector<Review> OrderManager::loadReviews(const std::string& restaurantName) const {
    std::vector<Review> reviews;
    std::ifstream file(reviewsFile);
    if (!file.is_open()) {
        return reviews; // No reviews file yet
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 5) continue;

        // Filter by restaurant name
        if (tokens[1] == restaurantName) {
            try {
                int rating = std::stoi(tokens[2]);
                Review review(tokens[0], tokens[1], rating, tokens[3], tokens[4]);
                reviews.push_back(review);
            } catch (...) {
                continue;
            }
        }
    }

    file.close();
    return reviews;
}

// Loads all reviews from file regardless of restaurant
// Returns complete vector of all Review objects
std::vector<Review> OrderManager::loadAllReviews() const {
    std::vector<Review> reviews;
    std::ifstream file(reviewsFile);
    if (!file.is_open()) {
        return reviews;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 5) continue;

        try {
            int rating = std::stoi(tokens[2]);
            Review review(tokens[0], tokens[1], rating, tokens[3], tokens[4]);
            reviews.push_back(review);
        } catch (...) {
            continue;
        }
    }

    file.close();
    return reviews;
}
