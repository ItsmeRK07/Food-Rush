#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "../models/DeliveryPartner.h"

// Composition: Manages pool of DeliveryPartner objects
// Demonstrates object lifecycle management
class DeliveryService {
private:
    std::vector<DeliveryPartner> partners;
    void initializePartners();

public:
    DeliveryService();
    DeliveryPartner* assignPartner();
    void completeDelivery(const std::string& partnerName);
    void ratePartner(const std::string& partnerName, double rating);
    void displayAvailablePartners() const;
    void displayAllPartners() const;
};
