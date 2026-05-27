#include "Restaurant.h"

// ============================================================================
// Restaurant Implementation
// OOP Concept: Composition - Restaurant owns its FoodItem menu and Review
//              collection. These are stored by value, ensuring deep copies.
// OOP Concept: Encapsulation - search, filter, and sort return new vectors
//              without modifying the internal menu state.
// ============================================================================

// Constructor
Restaurant::Restaurant(int id, const std::string& name, const std::string& location)
    : id(id), name(name), location(location) {}

// --- Getters (Encapsulation) ---
int Restaurant::getId() const { return id; }
std::string Restaurant::getName() const { return name; }
std::string Restaurant::getLocation() const { return location; }
const std::vector<FoodItem>& Restaurant::getMenu() const { return menu; }
const std::vector<Review>& Restaurant::getReviews() const { return reviews; }

// Composition: add items to the contained menu
void Restaurant::addFoodItem(const FoodItem& item) {
    menu.push_back(item);
}

// Composition: add reviews to the contained reviews collection
void Restaurant::addReview(const Review& review) {
    reviews.push_back(review);
}

// Compute average rating across all reviews
double Restaurant::getAverageRating() const {
    if (reviews.empty()) return 0.0;

    double sum = 0.0;
    for (const auto& review : reviews) {
        sum += review.getRating();
    }
    return sum / static_cast<double>(reviews.size());
}

// Display the full menu with header and rating
void Restaurant::displayMenu() const {
    std::cout << "\r\n  " << std::string(50, '-') << "\r\n";
    std::cout << "   " << name << " (" << location << ")\r\n";
    std::cout << "  " << std::string(50, '-') << "\r\n";

    // Show rating if reviews exist
    if (!reviews.empty()) {
        double avg = getAverageRating();
        int fullStars = static_cast<int>(avg);
        std::string stars = "";
        for (int i = 0; i < fullStars; ++i) stars += "\xE2\x98\x85";
        for (int i = fullStars; i < 5; ++i) stars += "\xE2\x98\x86";
        std::cout << "  Rating: " << stars << " ("
                  << std::fixed << std::setprecision(1) << avg << "/5)\r\n\r\n";
    }

    // Column headers
    std::cout << "   " << std::left << std::setw(4) << "#"
              << std::setw(32) << "Item"
              << "Price\r\n";
    std::cout << "  " << std::string(50, '-') << "\r\n";

    // List each menu item with 1-based index
    for (size_t i = 0; i < menu.size(); ++i) {
        std::cout << "   " << std::left << std::setw(4) << (i + 1)
                  << menu[i] << "\r\n";
    }
    std::cout << "  " << std::string(50, '-') << "\r\n";
}

// Display all reviews or a placeholder
void Restaurant::displayReviews() const {
    if (reviews.empty()) {
        std::cout << "\r\n  \xF0\x9F\x93\xAD No reviews yet.\r\n";
        return;
    }

    std::cout << "\r\n  " << std::string(50, '-') << "\r\n";
    std::cout << "   \xF0\x9F\x93\x9D Reviews for " << name << "\r\n";
    std::cout << "  " << std::string(50, '-') << "\r\n";

    for (const auto& review : reviews) {
        std::cout << review << "\r\n";
    }
    std::cout << "  " << std::string(50, '-') << "\r\n";
}

// Search menu by keyword (case-insensitive substring match)
std::vector<FoodItem> Restaurant::searchMenu(const std::string& keyword) const {
    std::vector<FoodItem> results;
    for (const auto& item : menu) {
        if (item.matchesSearch(keyword)) {
            results.push_back(item);
        }
    }
    return results;
}

// Filter menu to vegetarian items only
std::vector<FoodItem> Restaurant::filterVeg() const {
    std::vector<FoodItem> results;
    for (const auto& item : menu) {
        if (item.getIsVeg()) {
            results.push_back(item);
        }
    }
    return results;
}

// Return a copy of the menu sorted by price
std::vector<FoodItem> Restaurant::sortMenuByPrice(bool ascending) const {
    std::vector<FoodItem> sorted = menu;
    if (ascending) {
        std::sort(sorted.begin(), sorted.end(),
                  [](const FoodItem& a, const FoodItem& b) {
                      return a.getPrice() < b.getPrice();
                  });
    } else {
        std::sort(sorted.begin(), sorted.end(),
                  [](const FoodItem& a, const FoodItem& b) {
                      return a.getPrice() > b.getPrice();
                  });
    }
    return sorted;
}

// Operator Overloading: one-line restaurant summary
std::ostream& operator<<(std::ostream& os, const Restaurant& r) {
    os << "  " << r.id << ". " << std::left << std::setw(20) << r.name
       << " (" << r.location << ")";

    if (!r.reviews.empty()) {
        os << " | Rating: " << std::fixed << std::setprecision(1)
           << r.getAverageRating() << "/5";
    }

    os << " | " << r.menu.size() << " items";
    return os;
}
