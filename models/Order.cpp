#include "Order.h"

// ============================================================================
// Order Implementation
// OOP Concept: Composition - Order owns its FoodItem vector. When an Order
//              is copied or destroyed, the items go with it.
// OOP Concept: Encapsulation - total is computed internally; callers cannot
//              set an inconsistent total that doesn't match subtotal + fees.
// ============================================================================

// Static member initialization - auto-incrementing order IDs
int Order::nextOrderId = 1;

// Default Constructor
Order::Order()
    : orderId(0), customerName(""), restaurantName(""),
      subtotal(0.0), tax(0.0), deliveryFee(0.0), total(0.0),
      distance(0.0), estimatedTimeMinutes(0),
      status(OrderStatus::PLACED), deliveryPartnerName(""), timestamp("") {}

// Parameterized Constructor - auto-assigns ID and computes total
Order::Order(const std::string& customer, const std::string& restaurant,
             const std::vector<FoodItem>& items, double subtotal, double tax,
             double deliveryFee, double distance, int estTime,
             const std::string& partnerName)
    : customerName(customer), restaurantName(restaurant), items(items),
      subtotal(subtotal), tax(tax), deliveryFee(deliveryFee),
      distance(distance), estimatedTimeMinutes(estTime),
      status(OrderStatus::PLACED), deliveryPartnerName(partnerName) {

    // Auto-increment order ID
    orderId = nextOrderId++;

    // Compute total from components (Encapsulation: internal consistency)
    total = subtotal + tax + deliveryFee;

    // Generate timestamp
    time_t now = time(0);
    char buf[80];
    struct tm timeInfo;
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &timeInfo);
    timestamp = buf;
}

// --- Getters (Encapsulation) ---
int Order::getOrderId() const { return orderId; }
std::string Order::getCustomerName() const { return customerName; }
std::string Order::getRestaurantName() const { return restaurantName; }
const std::vector<FoodItem>& Order::getItems() const { return items; }
double Order::getSubtotal() const { return subtotal; }
double Order::getTax() const { return tax; }
double Order::getDeliveryFee() const { return deliveryFee; }
double Order::getTotal() const { return total; }
double Order::getDistance() const { return distance; }
int Order::getEstimatedTime() const { return estimatedTimeMinutes; }
OrderStatus Order::getStatus() const { return status; }
std::string Order::getDeliveryPartnerName() const { return deliveryPartnerName; }
std::string Order::getTimestamp() const { return timestamp; }

// Status to display string with emoji indicators
std::string Order::getStatusString() const {
    switch (status) {
        case OrderStatus::PLACED:
            return "\xF0\x9F\x93\xA6 Placed";
        case OrderStatus::PREPARING:
            return "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x8D\xB3 Preparing";
        case OrderStatus::OUT_FOR_DELIVERY:
            return "\xF0\x9F\x9A\x97 Out for Delivery";
        case OrderStatus::DELIVERED:
            return "\xE2\x9C\x85 Delivered";
        default:
            return "Unknown";
    }
}

// Setters - controlled mutation
void Order::setStatus(OrderStatus s) { status = s; }
void Order::setOrderId(int id) { orderId = id; }

// One-line summary for order history lists
void Order::displaySummary() const {
    std::cout << "  #" << std::left << std::setw(5) << orderId
              << " | " << std::setw(20) << restaurantName
              << " | \xE2\x82\xB9" << std::right << std::fixed << std::setprecision(2) << std::setw(8) << total
              << " | " << getStatusString() << "\r\n";
}

// Full detailed order display with all information
void Order::displayDetailed() const {
    std::cout << "\r\n  " << std::string(50, '=') << "\r\n";
    std::cout << "   \xF0\x9F\x93\x8B Order #" << orderId << " - Details\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";

    std::cout << "  Customer      : " << customerName << "\r\n";
    std::cout << "  Restaurant    : " << restaurantName << "\r\n";
    std::cout << "  Status        : " << getStatusString() << "\r\n";
    std::cout << "  Ordered At    : " << timestamp << "\r\n";
    std::cout << "  Delivery By   : " << deliveryPartnerName << "\r\n";

    std::cout << "\r\n  " << std::string(40, '-') << "\r\n";
    std::cout << "   Items Ordered:\r\n";
    std::cout << "  " << std::string(40, '-') << "\r\n";

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << items[i] << "\r\n";
    }

    std::cout << "\r\n  " << std::string(40, '-') << "\r\n";
    std::cout << "   Price Breakdown:\r\n";
    std::cout << "  " << std::string(40, '-') << "\r\n";
    std::cout << "  Subtotal       : \xE2\x82\xB9" << std::fixed << std::setprecision(2) << subtotal << "\r\n";
    std::cout << "  Tax (GST)      : \xE2\x82\xB9" << std::fixed << std::setprecision(2) << tax << "\r\n";
    std::cout << "  Delivery Fee   : \xE2\x82\xB9" << std::fixed << std::setprecision(2) << deliveryFee << "\r\n";
    std::cout << "  " << std::string(30, '-') << "\r\n";
    std::cout << "  \xF0\x9F\x92\xB0 Total        : \xE2\x82\xB9" << std::fixed << std::setprecision(2) << total << "\r\n";

    std::cout << "\r\n  Distance       : " << std::fixed << std::setprecision(1) << distance << " km\r\n";
    std::cout << "  Estimated Time : " << estimatedTimeMinutes << " minutes\r\n";
    std::cout << "  " << std::string(50, '=') << "\r\n";
}

// Operator Overloading: delegates to summary display
std::ostream& operator<<(std::ostream& os, const Order& order) {
    order.displaySummary();
    return os;
}
