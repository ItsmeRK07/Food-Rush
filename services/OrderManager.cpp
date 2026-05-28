#include "OrderManager.h"
#include "../utils/FoodRushException.h"
#include <algorithm>
#include <ctime>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

OrderManager::OrderManager()
    : ordersFile("data/orders.txt"),
      reviewsFile("data/reviews.txt"),
      taxRate(0.05),
      baseDeliveryFee(10.0),
      deliveryFeePerKm(5.0) {
    MKDIR("data");
}

Order* OrderManager::placeOrder(Customer& customer, Cart& cart, RouteManager& routeManager,
                                 DeliveryService& deliveryService, AuthService& authService,
                                 const std::string& userLocation, const std::string& restaurantLocation,
                                 const std::string& restaurantName) {
    if (cart.isEmpty()) {
        throw OrderException("Your cart is empty! Add items before placing an order.");
    }

    double distance = routeManager.findShortestDistance(restaurantLocation, userLocation);

    double subtotal = cart.calculateSubtotal();
    double tax = subtotal * taxRate;
    double deliveryFee = baseDeliveryFee + (distance * deliveryFeePerKm);
    double total = subtotal + tax + deliveryFee;
    int estTime = routeManager.estimateDeliveryTime(distance);

    DeliveryPartner* partner = deliveryService.assignPartner();
    if (partner == nullptr) {
        throw OrderException("No delivery partners available right now. Please try again later.");
    }

    if (customer.getWalletBalance() < total) {
        std::ostringstream oss;
        oss << "Insufficient funds! Required: \xE2\x82\xB9" << std::fixed << std::setprecision(2) << total
            << " | Available: \xE2\x82\xB9" << customer.getWalletBalance();
        throw OrderException(oss.str());
    }

    customer.deductFunds(total);
    authService.updateWalletBalance(customer.getUsername(), customer.getWalletBalance());

    auto flatItems = cart.getItemsFlat();
    Order order(customer.getUsername(), restaurantName, flatItems,
                subtotal, tax, deliveryFee, distance, estTime,
                partner->getUsername());
    order.setStatus(OrderStatus::PLACED);

    std::string partnerName = partner->getUsername();
    partner->assignOrder(order.getOrderId());

    customer.addOrder(order);
    allOrders.push_back(order);
    saveOrdersToFile();

    std::cout << "\n  \xE2\x95\x94";
    for (int i = 0; i < 38; ++i) std::cout << "\xE2\x95\x90";
    std::cout << "\xE2\x95\x97" << std::endl;
    std::cout << "  \xE2\x95\x91       \xF0\x9F\xA7\xBE CHECKOUT SUMMARY            \xE2\x95\x91" << std::endl;
    std::cout << "  \xE2\x95\x9A";
    for (int i = 0; i < 38; ++i) std::cout << "\xE2\x95\x90";
    std::cout << "\xE2\x95\x9D" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Subtotal:      \xE2\x82\xB9" << subtotal << std::endl;
    std::cout << "  Tax (5%):      \xE2\x82\xB9" << tax << std::endl;
    std::cout << "  Distance:      " << std::setprecision(1) << distance << " km" << std::endl;
    std::cout << "  Delivery Fee:  \xE2\x82\xB9" << std::setprecision(2) << deliveryFee << std::endl;
    std::cout << "  ";
    for (int i = 0; i < 39; ++i) std::cout << "\xE2\x94\x80";
    std::cout << std::endl;
    std::cout << "  Total:         \xE2\x82\xB9" << total << std::endl;
    std::cout << std::endl;
    std::cout << "  \xE2\x9C\x85 Order #" << order.getOrderId() << " Placed Successfully!" << std::endl;
    std::cout << "  \xF0\x9F\x9A\x97 Delivery Partner: " << partnerName << std::endl;
    std::cout << "  \xE2\x8F\xB1  Estimated Delivery: " << estTime << " minutes" << std::endl;

    routeManager.displayRoute(restaurantLocation, userLocation);

    std::cout << "\n  Remaining Balance: \xE2\x82\xB9" << customer.getWalletBalance() << std::endl;
    std::cout << std::endl;

    cart.clearCart();
    return &allOrders.back();
}

bool OrderManager::cancelOrder(int orderId, Customer& customer, AuthService& authService,
                                DeliveryService& deliveryService) {
    Order* order = getOrder(orderId);
    if (!order) {
        throw OrderException("Order #" + std::to_string(orderId) + " not found!");
    }
    if (order->getCustomerName() != customer.getUsername()) {
        throw OrderException("Order #" + std::to_string(orderId) + " does not belong to you!");
    }
    OrderStatus status = order->getStatus();
    if (status == OrderStatus::DELIVERED) {
        throw OrderException("Order #" + std::to_string(orderId) + " has already been delivered and cannot be cancelled.");
    }
    if (status == OrderStatus::CANCELLED) {
        throw OrderException("Order #" + std::to_string(orderId) + " is already cancelled.");
    }
    if (status == OrderStatus::OUT_FOR_DELIVERY) {
        throw OrderException("Order #" + std::to_string(orderId) + " is already out for delivery and cannot be cancelled.");
    }

    double refundAmount = order->getTotal();
    order->setStatus(OrderStatus::CANCELLED);
    customer.addFunds(refundAmount);
    authService.updateWalletBalance(customer.getUsername(), customer.getWalletBalance());
    deliveryService.completeDelivery(order->getDeliveryPartnerName());
    saveOrdersToFile();
    return true;
}

std::vector<Order> OrderManager::getOrderHistory(const std::string& customerName) const {
    std::vector<Order> history;
    for (const auto& order : allOrders) {
        if (order.getCustomerName() == customerName) history.push_back(order);
    }
    return history;
}

Order* OrderManager::getOrder(int orderId) {
    for (auto& order : allOrders) {
        if (order.getOrderId() == orderId) return &order;
    }
    return nullptr;
}

void OrderManager::trackOrder(int orderId) const {
    for (const auto& order : allOrders) {
        if (order.getOrderId() == orderId) { order.displayDetailed(); return; }
    }
    std::cout << "\n  \xE2\x9D\x8C Order #" << orderId << " not found!" << std::endl;
}

void OrderManager::saveOrdersToFile() const {
    std::ofstream file(ordersFile, std::ios::trunc);
    if (!file.is_open()) return;
    for (const auto& order : allOrders) {
        file << order.getOrderId() << "|" << order.getCustomerName() << "|"
             << order.getRestaurantName() << "|" << std::fixed << std::setprecision(2)
             << order.getSubtotal() << "|" << order.getTax() << "|"
             << order.getDeliveryFee() << "|" << order.getTotal() << "|"
             << order.getDistance() << "|" << order.getEstimatedTime() << "|"
             << static_cast<int>(order.getStatus()) << "|"
             << order.getDeliveryPartnerName() << "|" << order.getTimestamp() << "|";
        const auto& items = order.getItems();
        for (size_t i = 0; i < items.size(); i++) {
            file << items[i].getName() << ":" << items[i].getPrice()
                 << ":" << (items[i].getIsVeg() ? 1 : 0);
            if (i < items.size() - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}

void OrderManager::loadOrdersFromFile() {
    std::ifstream file(ordersFile);
    if (!file.is_open()) return;

    std::string line;
    int maxOrderId = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(iss, token, '|')) tokens.push_back(token);
        if (tokens.size() < 13) continue;
        try {
            int orderId = std::stoi(tokens[0]);
            std::string customerName = tokens[1];
            std::string restName = tokens[2];
            double subtotal = std::stod(tokens[3]);
            double tax = std::stod(tokens[4]);
            double deliveryFee = std::stod(tokens[5]);
            // tokens[6] is total, not used in constructor since it derives from others
            double distance = std::stod(tokens[7]);
            int estTime = std::stoi(tokens[8]);
            int statusInt = std::stoi(tokens[9]);
            std::string partnerName = tokens[10];
            std::string timestamp = tokens[11];

            std::vector<FoodItem> items;
            if (!tokens[12].empty()) {
                std::istringstream itemStream(tokens[12]);
                std::string itemToken;
                while (std::getline(itemStream, itemToken, ',')) {
                    size_t lastColon = itemToken.rfind(':');
                    if (lastColon == std::string::npos) continue;
                    size_t secondColon = itemToken.rfind(':', lastColon - 1);
                    bool isVeg = true;
                    std::string itemName;
                    double itemPrice;
                    if (secondColon != std::string::npos && secondColon != lastColon) {
                        itemName = itemToken.substr(0, secondColon);
                        itemPrice = std::stod(itemToken.substr(secondColon + 1, lastColon - secondColon - 1));
                        isVeg = (itemToken.substr(lastColon + 1) == "1");
                    } else {
                        itemName = itemToken.substr(0, lastColon);
                        itemPrice = std::stod(itemToken.substr(lastColon + 1));
                    }
                    items.push_back(FoodItem(0, itemName, itemPrice, isVeg));
                }
            }

            Order order(customerName, restName, items, subtotal, tax, deliveryFee, distance, estTime, partnerName);
            order.setOrderId(orderId);
            order.setStatus(static_cast<OrderStatus>(statusInt));
            allOrders.push_back(order);
            if (orderId > maxOrderId) maxOrderId = orderId;
            Order::setNextOrderId(maxOrderId + 1);
        } catch (const std::exception& e) { continue; }
    }
    file.close();
}

void OrderManager::saveReview(const Review& review) const {
    MKDIR("data");
    std::ofstream file(reviewsFile, std::ios::app);
    if (!file.is_open()) return;
    file << review.getReviewerName() << "|" << review.getRestaurantName() << "|"
         << review.getRating() << "|" << review.getComment() << "|"
         << review.getTimestamp() << "\n";
    file.close();
}

std::vector<Review> OrderManager::loadReviews(const std::string& restaurantName) const {
    std::vector<Review> reviews;
    std::ifstream file(reviewsFile);
    if (!file.is_open()) return reviews;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(iss, token, '|')) tokens.push_back(token);
        if (tokens.size() < 5) continue;
        if (tokens[1] == restaurantName) {
            try {
                int rating = std::stoi(tokens[2]);
                reviews.push_back(Review(tokens[0], tokens[1], rating, tokens[3]));
            } catch (...) { continue; }
        }
    }
    file.close();
    return reviews;
}

std::vector<Review> OrderManager::loadAllReviews() const {
    std::vector<Review> reviews;
    std::ifstream file(reviewsFile);
    if (!file.is_open()) return reviews;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(iss, token, '|')) tokens.push_back(token);
        if (tokens.size() < 5) continue;
        try {
            int rating = std::stoi(tokens[2]);
            reviews.push_back(Review(tokens[0], tokens[1], rating, tokens[3]));
        } catch (...) { continue; }
    }
    file.close();
    return reviews;
}
