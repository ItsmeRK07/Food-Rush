#include "Review.h"
#include <algorithm>

// ============================================================================
// Review Implementation
// OOP Concept: Encapsulation - rating is clamped to valid range internally;
// callers cannot set an invalid rating through the constructor.
// ============================================================================

// Default Constructor
Review::Review() : reviewerName(""), restaurantName(""), rating(0), comment(""), timestamp("") {}

// Parameterized Constructor - auto-generates timestamp
Review::Review(const std::string& reviewer, const std::string& restaurant,
               int rating, const std::string& comment)
    : reviewerName(reviewer), restaurantName(restaurant), comment(comment) {
    // Clamp rating to valid [1, 5] range
    this->rating = std::max(1, std::min(5, rating));

    // Generate timestamp at review creation time
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
std::string Review::getReviewerName() const { return reviewerName; }
std::string Review::getRestaurantName() const { return restaurantName; }
int Review::getRating() const { return rating; }
std::string Review::getComment() const { return comment; }
std::string Review::getTimestamp() const { return timestamp; }

// Build visual star string: ★★★☆☆
std::string Review::getRatingStars() const {
    std::string stars = "";
    for (int i = 0; i < rating; ++i)
        stars += "\xE2\x98\x85";  // ★
    for (int i = rating; i < 5; ++i)
        stars += "\xE2\x98\x86";  // ☆
    return stars;
}

// Operator Overloading: formatted review output
std::ostream& operator<<(std::ostream& os, const Review& review) {
    os << "  " << review.reviewerName << " | " << review.getRatingStars()
       << " | " << review.comment << " | " << review.timestamp;
    return os;
}
