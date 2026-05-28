#include "Cart.h"
#include "../utils/FoodRushException.h"

// ============================================================================
// Cart Implementation (Quantity-Aware with std::map)
// OOP Concept: Composition - Cart owns its FoodItem->quantity mapping.
// OOP Concept: Encapsulation - the single-restaurant constraint is enforced
//              internally in addItem(); callers don't need to check this.
// DSA: std::map<FoodItem, int> uses FoodItem::operator< for key ordering,
//      providing O(log n) operations and automatic deduplication.
// ============================================================================

// Default Constructor
Cart::Cart() : currentRestaurant("") {}

// Add item with single-restaurant validation — increments qty if already present
bool Cart::addItem(const FoodItem& item, const std::string& restaurantName) {
    if (items.empty()) {
        currentRestaurant = restaurantName;
    }
    if (currentRestaurant != restaurantName) {
        throw ValidationException("You can only order from one restaurant at a time. Clear your cart to switch restaurants.");
    }
    items[item]++;
    std::cout << "  \xE2\x9C\x85 Added \"" << item.getName() << "\" to cart. (Qty: " << items[item] << ")\r\n";
    return true;
}

bool Cart::removeItem(const FoodItem& item) {
    auto it = items.find(item);
    if (it == items.end()) {
        throw ValidationException("Item not found in cart.");
    }
    std::cout << "  \xE2\x9C\x85 Removed \"" << it->first.getName() << "\" from cart.\r\n";
    items.erase(it);
    if (items.empty()) { currentRestaurant = ""; }
    return true;
}

void Cart::incrementItem(const FoodItem& item) {
    auto it = items.find(item);
    if (it == items.end()) { throw ValidationException("Item not found in cart."); }
    it->second++;
    std::cout << "  \xE2\x9C\x85 \"" << it->first.getName() << "\" qty increased to " << it->second << ".\r\n";
}

void Cart::decrementItem(const FoodItem& item) {
    auto it = items.find(item);
    if (it == items.end()) { throw ValidationException("Item not found in cart."); }
    it->second--;
    if (it->second <= 0) {
        std::cout << "  \xE2\x9C\x85 \"" << it->first.getName() << "\" removed from cart.\r\n";
        items.erase(it);
        if (items.empty()) { currentRestaurant = ""; }
    } else {
        std::cout << "  \xE2\x9C\x85 \"" << it->first.getName() << "\" qty decreased to " << it->second << ".\r\n";
    }
}

double Cart::calculateSubtotal() const {
    double subtotal = 0.0;
    for (auto it = items.begin(); it != items.end(); ++it) {
        subtotal += it->first.getPrice() * it->second;
    }
    return subtotal;
}

void Cart::displayCart() const {
    if (items.empty()) {
        std::cout << "\r\n  \xF0\x9F\x9B\x92 Your cart is empty.\r\n";
        return;
    }
    std::cout << "\r\n  " << std::string(50, '=') << "\r\n";
    std::cout << "   \xF0\x9F\x9B\x92 Your Cart - " << currentRestaurant << "\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
    int idx = 1;
    for (auto it = items.begin(); it != items.end(); ++it) {
        double lineTotal = it->first.getPrice() * it->second;
        std::cout << "   " << std::left << std::setw(4) << idx++
                  << it->first << " x" << it->second
                  << "  = \xE2\x82\xB9" << std::fixed << std::setprecision(2) << lineTotal << "\r\n";
    }
    std::cout << "  " << std::string(50, '-') << "\r\n";
    std::cout << "   Subtotal: \xE2\x82\xB9" << std::fixed << std::setprecision(2) << calculateSubtotal() << "\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
}

void Cart::clearCart() { items.clear(); currentRestaurant = ""; }
bool Cart::isEmpty() const { return items.empty(); }

int Cart::getItemCount() const {
    int count = 0;
    for (auto it = items.begin(); it != items.end(); ++it) { count += it->second; }
    return count;
}

const std::map<FoodItem, int>& Cart::getItems() const { return items; }
std::string Cart::getCurrentRestaurant() const { return currentRestaurant; }

std::vector<FoodItem> Cart::getItemsFlat() const {
    std::vector<FoodItem> flat;
    for (auto it = items.begin(); it != items.end(); ++it) {
        for (int i = 0; i < it->second; ++i) {
            flat.push_back(it->first);
        }
    }
    return flat;
}

std::ostream& operator<<(std::ostream& os, const Cart& cart) {
    if (cart.items.empty()) {
        os << "  \xF0\x9F\x9B\x92 Your cart is empty.";
        return os;
    }
    os << "\r\n  " << std::string(50, '=') << "\r\n";
    os << "   \xF0\x9F\x9B\x92 Cart - " << cart.currentRestaurant << "\r\n";
    os << "  " << std::string(50, '=') << "\r\n";
    int idx = 1;
    for (auto it = cart.items.begin(); it != cart.items.end(); ++it) {
        double lineTotal = it->first.getPrice() * it->second;
        os << "   " << std::left << std::setw(4) << idx++
           << it->first << " x" << it->second
           << "  = \xE2\x82\xB9" << std::fixed << std::setprecision(2) << lineTotal << "\r\n";
    }
    os << "  " << std::string(50, '-') << "\r\n";
    os << "   Subtotal: \xE2\x82\xB9" << std::fixed << std::setprecision(2) << cart.calculateSubtotal() << "\r\n";
    os << "  " << std::string(50, '=');
    return os;
}
