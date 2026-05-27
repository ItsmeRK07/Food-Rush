#include "DeliveryService.h"
#include <iomanip>

// Constructor: Initializes the delivery partner pool
// Composition: DeliveryService "has-a" collection of DeliveryPartner objects
DeliveryService::DeliveryService() {
    initializePartners();
}

// Encapsulation: Partner initialization is a private implementation detail
// Creates 4 hardcoded delivery partners
void DeliveryService::initializePartners() {
    partners.push_back(DeliveryPartner("Rahul_D", "pass"));
    partners.push_back(DeliveryPartner("Amit_R", "pass"));
    partners.push_back(DeliveryPartner("Priya_S", "pass"));
    partners.push_back(DeliveryPartner("Vikram_K", "pass"));
}

// Assigns the first available delivery partner to an order
// Object Lifecycle: Transitions partner state from available to busy
// Returns pointer to the assigned partner, or nullptr if all are busy
DeliveryPartner* DeliveryService::assignPartner() {
    for (auto& partner : partners) {
        if (partner.getIsAvailable()) {
            return &partner;
        }
    }
    std::cout << "\n  \u274C All delivery partners are busy!" << std::endl;
    return nullptr;
}

// Marks a delivery as complete for the specified partner
// Object Lifecycle: Transitions partner state back to available
void DeliveryService::completeDelivery(const std::string& partnerName) {
    for (auto& partner : partners) {
        if (partner.getUsername() == partnerName) {
            partner.completeDelivery();
            return;
        }
    }
    std::cout << "\n  \u274C Delivery partner '" << partnerName << "' not found!" << std::endl;
}

// Adds a rating for a specific delivery partner
// Encapsulation: Rating calculation is delegated to DeliveryPartner object
void DeliveryService::ratePartner(const std::string& partnerName, double rating) {
    for (auto& partner : partners) {
        if (partner.getUsername() == partnerName) {
            partner.addRating(rating);
            std::cout << "\n  \u2B50 Rating submitted for " << partnerName << "!" << std::endl;
            return;
        }
    }
    std::cout << "\n  \u274C Delivery partner '" << partnerName << "' not found!" << std::endl;
}

// Displays only partners who are currently available for delivery
void DeliveryService::displayAvailablePartners() const {
    std::cout << "\n  \U0001F69A Available Delivery Partners:" << std::endl;
    std::cout << "  \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500" << std::endl;

    bool found = false;
    for (const auto& partner : partners) {
        if (partner.getIsAvailable()) {
            std::cout << "  \u2705 " << partner.getUsername()
                      << "  \u2B50 " << std::fixed << std::setprecision(1) << partner.getRating()
                      << "  \U0001F4E6 " << partner.getTotalDeliveries() << " deliveries" << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "  \u274C No partners available at the moment." << std::endl;
    }
    std::cout << std::endl;
}

// Displays all partners with their complete stats
// Polymorphism: Uses displayDashboard() which is overridden in DeliveryPartner
void DeliveryService::displayAllPartners() const {
    std::cout << "\n  \U0001F69A All Delivery Partners:" << std::endl;
    std::cout << "  \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500" << std::endl;

    for (const auto& partner : partners) {
        std::string status = partner.getIsAvailable() ? "\u2705 Available" : "\U0001F6D1 Busy";
        std::cout << "  " << status
                  << "  " << partner.getUsername()
                  << "  \u2B50 " << std::fixed << std::setprecision(1) << partner.getRating()
                  << "  \U0001F4E6 " << partner.getTotalDeliveries() << " deliveries" << std::endl;
    }
    std::cout << std::endl;
}
