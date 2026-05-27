#include "DeliveryPartner.h"
#include <sstream>

// ============================================================================
// DeliveryPartner Implementation
// OOP Concept: Inheritance - calls User(uName, pass) for shared init.
// OOP Concept: Polymorphism - displayDashboard() and toString() override
//              base class versions for delivery-partner-specific behavior.
// ============================================================================

// Constructor: delegates to User base, initializes partner-specific fields
DeliveryPartner::DeliveryPartner(const std::string& uName, const std::string& pass)
    : User(uName, pass), isAvailable(true), rating(0.0),
      totalDeliveries(0), currentOrderId(-1) {}

// --- Getters (Encapsulation) ---
bool DeliveryPartner::getIsAvailable() const { return isAvailable; }
double DeliveryPartner::getRating() const { return rating; }
int DeliveryPartner::getTotalDeliveries() const { return totalDeliveries; }
int DeliveryPartner::getCurrentOrderId() const { return currentOrderId; }

// Assign an order - partner becomes busy
void DeliveryPartner::assignOrder(int orderId) {
    isAvailable = false;
    currentOrderId = orderId;
}

// Complete the current delivery - partner becomes available again
void DeliveryPartner::completeDelivery() {
    isAvailable = true;
    currentOrderId = -1;
    totalDeliveries++;
}

// Update running average rating
void DeliveryPartner::addRating(double newRating) {
    if (totalDeliveries > 0) {
        // Running average: fold in the new rating with existing average
        rating = ((rating * (totalDeliveries - 1)) + newRating) / totalDeliveries;
    } else {
        rating = newRating;
    }
}

// Polymorphism: DeliveryPartner-specific dashboard
void DeliveryPartner::displayDashboard() {
    std::cout << "\r\n  " << std::string(50, '=') << "\r\n";
    std::cout << "   \xF0\x9F\x9A\x97 Delivery Partner Dashboard\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
    std::cout << "  Partner Name   : " << username << "\r\n";
    std::cout << "  Status         : " << (isAvailable ? "\xF0\x9F\x9F\xA2 Available" : "\xF0\x9F\x94\xB4 Busy") << "\r\n";

    if (totalDeliveries > 0) {
        std::cout << "  \xE2\xAD\x90 Rating        : " << std::fixed << std::setprecision(1) << rating << "/5\r\n";
    } else {
        std::cout << "  \xE2\xAD\x90 Rating        : N/A (No deliveries yet)\r\n";
    }

    std::cout << "  \xF0\x9F\x93\xA6 Deliveries    : " << totalDeliveries << "\r\n";

    if (!isAvailable && currentOrderId != -1) {
        std::cout << "  \xF0\x9F\x93\x8B Current Order  : #" << currentOrderId << "\r\n";
    }

    std::cout << "  " << std::string(50, '=') << "\r\n";
}

// Polymorphism: DeliveryPartner-specific toString
std::string DeliveryPartner::toString() const {
    std::ostringstream oss;
    oss << "Partner: " << username << " | " << (isAvailable ? "Available" : "Busy");
    return oss.str();
}
