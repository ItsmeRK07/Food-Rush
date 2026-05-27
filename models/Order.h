#pragma once
#include <iostream>
#include <string>
#include "Cart.h"
#include "Customer.h"
#include "../services/RouteManager.h"

class Order {
private:
    Cart cart;
    double taxRate = 0.05;
    double deliveryFeePerKm = 1.50; 

public:
    void placeOrder(Customer& customer, Cart& userCart, RouteManager& map, std::string userLocation, std::string restaurantLocation) {
        if (userCart.isEmpty()) {
            std::cout << "❌ Cannot place an empty order.\n";
            return;
        }

        double distance = map.findShortestDistance(restaurantLocation, userLocation);
        
        if (distance < 0) {
            std::cout << "❌ Delivery unavailable to this location.\n";
            return;
        }

        double subtotal = userCart.calculateSubtotal();
        double tax = subtotal * taxRate;
        double totalDeliveryFee = distance * deliveryFeePerKm;
        double totalBill = subtotal + tax + totalDeliveryFee;

        std::cout << "\n--- Checkout Summary ---\n";
        std::cout << "Subtotal: $" << subtotal << "\n";
        std::cout << "Taxes (5%): $" << tax << "\n";
        std::cout << "Distance: " << distance << " km\n";
        std::cout << "Delivery Fee: $" << totalDeliveryFee << "\n";
        std::cout << "Total Bill: $" << totalBill << "\n";

        if (customer.getWalletBalance() >= totalBill) {
            customer.addFunds(-totalBill);
            std::cout << "\n✅ Order Placed Successfully!\n";
            std::cout << "🚕 Assigning driver for the " << distance << " km route...\n";
            std::cout << "Remaining Wallet Balance: $" << customer.getWalletBalance() << "\n";
            userCart.clearCart();
        } else {
            std::cout << "\n❌ Payment Failed: Insufficient funds.\n";
        }
    }
};