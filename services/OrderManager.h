#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "../models/Order.h"
#include "../models/Customer.h"
#include "../models/Cart.h"
#include "../models/Review.h"
#include "RouteManager.h"
#include "DeliveryService.h"
#include "AuthService.h"

// Service-Oriented: Orchestrates the complete order workflow
// Composition: Coordinates multiple services (Route, Delivery, Auth)
class OrderManager {
private:
    std::vector<Order> allOrders;
    std::string ordersFile;
    std::string reviewsFile;
    double taxRate;
    double baseDeliveryFee;
    double deliveryFeePerKm;

public:
    OrderManager();

    // Updated signature: includes restaurantLocation for route calculation
    Order* placeOrder(Customer& customer, Cart& cart, RouteManager& routeManager,
                      DeliveryService& deliveryService, AuthService& authService,
                      const std::string& userLocation, const std::string& restaurantLocation,
                      const std::string& restaurantName);

    // Order cancellation with auto-refund
    bool cancelOrder(int orderId, Customer& customer, AuthService& authService,
                     DeliveryService& deliveryService);

    std::vector<Order> getOrderHistory(const std::string& customerName) const;
    Order* getOrder(int orderId);
    void trackOrder(int orderId) const;
    void saveOrdersToFile() const;
    void loadOrdersFromFile();
    void saveReview(const Review& review) const;
    std::vector<Review> loadReviews(const std::string& restaurantName) const;
    std::vector<Review> loadAllReviews() const;
};
