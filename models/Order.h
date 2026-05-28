#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "FoodItem.h"

// ============================================================================
// OrderStatus - Enum Class for Type Safety
// Using enum class instead of plain enum prevents implicit conversions
// and provides scoped constants (OrderStatus::PLACED, not just PLACED).
// ============================================================================
enum class OrderStatus { PLACED, PREPARING, OUT_FOR_DELIVERY, DELIVERED, CANCELLED };

// ============================================================================
// Order - Rich Data Model
// OOP Concept: Composition - Order contains a vector of FoodItems.
//              The Order "has-a" collection of items, not "is-a" FoodItem.
// OOP Concept: Encapsulation - complex pricing logic and status management
//              are hidden behind a clean public interface.
// ============================================================================
class Order {
private:
    int orderId;
    std::string customerName;
    std::string restaurantName;
    std::vector<FoodItem> items;
    double subtotal;
    double tax;
    double deliveryFee;
    double total;
    double distance;
    int estimatedTimeMinutes;
    OrderStatus status;
    std::string deliveryPartnerName;
    std::string timestamp;

    // Static counter for auto-incrementing order IDs
    static int nextOrderId;

public:
    Order();
    Order(const std::string& customer, const std::string& restaurant,
          const std::vector<FoodItem>& items, double subtotal, double tax,
          double deliveryFee, double distance, int estTime,
          const std::string& partnerName);

    // Getters - Encapsulation
    int getOrderId() const;
    std::string getCustomerName() const;
    std::string getRestaurantName() const;
    const std::vector<FoodItem>& getItems() const;
    double getSubtotal() const;
    double getTax() const;
    double getDeliveryFee() const;
    double getTotal() const;
    double getDistance() const;
    int getEstimatedTime() const;
    OrderStatus getStatus() const;
    std::string getStatusString() const;
    std::string getDeliveryPartnerName() const;
    std::string getTimestamp() const;

    // Setters - controlled mutation
    void setStatus(OrderStatus s);
    void setOrderId(int id);

    // Static method to sync nextOrderId after loading from file
    static void setNextOrderId(int id);

    // Display methods
    void displaySummary() const;
    void displayDetailed() const;

    // Operator Overloading: formatted stream output
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};