#include "FoodItem.h"

// ============================================================================
// FoodItem Implementation
// OOP Concept: Encapsulation - all data access goes through the public API
// OOP Concept: Operator Overloading - natural syntax for comparison & output
// ============================================================================

// Default Constructor
FoodItem::FoodItem() : id(0), name(""), price(0.0), isVeg(false) {}

// Parameterized Constructor
FoodItem::FoodItem(int id, const std::string& name, double price, bool isVeg)
    : id(id), name(name), price(price), isVeg(isVeg) {}

// --- Getters (Encapsulation) ---
int FoodItem::getId() const { return id; }
std::string FoodItem::getName() const { return name; }
double FoodItem::getPrice() const { return price; }
bool FoodItem::getIsVeg() const { return isVeg; }

// Case-insensitive substring search
bool FoodItem::matchesSearch(const std::string& keyword) const {
    std::string lowerName = name;
    std::string lowerKeyword = keyword;

    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return lowerName.find(lowerKeyword) != std::string::npos;
}

// Operator Overloading: compare FoodItems by their unique id
bool FoodItem::operator==(const FoodItem& other) const {
    return id == other.id;
}

// Operator Overloading: formatted stream output
// Displays veg/non-veg indicator and right-aligned price in ₹
std::ostream& operator<<(std::ostream& os, const FoodItem& item) {
    std::string tag = item.isVeg ? "\xF0\x9F\x9F\xA2" : "\xF0\x9F\x94\xB4";
    os << "  " << tag << " " << std::left << std::setw(28) << item.name
       << "\xE2\x82\xB9" << std::right << std::fixed << std::setprecision(2) << item.price;
    return os;
}
