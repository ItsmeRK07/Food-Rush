#include "Cart.h"

// ============================================================================
// Cart Implementation
// OOP Concept: Composition - Cart owns its FoodItem collection.
// OOP Concept: Encapsulation - the single-restaurant constraint is enforced
//              internally in addItem(); callers don't need to check this.
// ============================================================================

// Default Constructor
Cart::Cart() : currentRestaurant("") {}

// Add item with single-restaurant validation
bool Cart::addItem(const FoodItem& item, const std::string& restaurantName) {
    // First item sets the restaurant context
    if (items.empty()) {
        currentRestaurant = restaurantName;
    }

    // Enforce single-restaurant rule (Encapsulation of business logic)
    if (currentRestaurant != restaurantName) {
        std::cout << "  \xE2\x9D\x8C You can only order from one restaurant at a time.\r\n";
        std::cout << "     Clear your cart to switch restaurants.\r\n";
        return false;
    }

    items.push_back(item);
    std::cout << "  \xE2\x9C\x85 Added \"" << item.getName() << "\" to cart.\r\n";
    return true;
}

// Remove item by 0-based index with bounds checking
bool Cart::removeItem(int index) {
    if (index < 0 || index >= static_cast<int>(items.size())) {
        std::cout << "  \xE2\x9D\x8C Invalid item number.\r\n";
        return false;
    }

    std::cout << "  \xE2\x9C\x85 Removed \"" << items[index].getName() << "\" from cart.\r\n";
    items.erase(items.begin() + index);

    // Reset restaurant context if cart becomes empty
    if (items.empty()) {
        currentRestaurant = "";
    }

    return true;
}

// Sum all item prices
double Cart::calculateSubtotal() const {
    double subtotal = 0.0;
    for (const auto& item : items) {
        subtotal += item.getPrice();
    }
    return subtotal;
}

// Display cart contents with subtotal
void Cart::displayCart() const {
    if (items.empty()) {
        std::cout << "\r\n  \xF0\x9F\x9B\x92 Your cart is empty.\r\n";
        return;
    }

    std::cout << "\r\n  " << std::string(50, '=') << "\r\n";
    std::cout << "   \xF0\x9F\x9B\x92 Your Cart - " << currentRestaurant << "\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "   " << std::left << std::setw(4) << (i + 1)
                  << items[i] << "\r\n";
    }

    std::cout << "  " << std::string(50, '-') << "\r\n";
    std::cout << "   Subtotal: \xE2\x82\xB9" << std::fixed << std::setprecision(2)
              << calculateSubtotal() << "\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
}

// Clear all items and reset restaurant context
void Cart::clearCart() {
    items.clear();
    currentRestaurant = "";
}

bool Cart::isEmpty() const { return items.empty(); }

// --- Getters (Encapsulation) ---
const std::vector<FoodItem>& Cart::getItems() const { return items; }
std::string Cart::getCurrentRestaurant() const { return currentRestaurant; }

// Operator Overloading: delegates to displayCart-style output
std::ostream& operator<<(std::ostream& os, const Cart& cart) {
    if (cart.items.empty()) {
        os << "  \xF0\x9F\x9B\x92 Your cart is empty.";
        return os;
    }

    os << "\r\n  " << std::string(50, '=') << "\r\n";
    os << "   \xF0\x9F\x9B\x92 Cart - " << cart.currentRestaurant << "\r\n";
    os << "  " << std::string(50, '=') << "\r\n";

    for (size_t i = 0; i < cart.items.size(); ++i) {
        os << "   " << std::left << std::setw(4) << (i + 1)
           << cart.items[i] << "\r\n";
    }

    os << "  " << std::string(50, '-') << "\r\n";
    os << "   Subtotal: \xE2\x82\xB9" << std::fixed << std::setprecision(2)
       << cart.calculateSubtotal() << "\r\n";
    os << "  " << std::string(50, '=');
    return os;
}
