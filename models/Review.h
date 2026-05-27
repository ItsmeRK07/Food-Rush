#pragma once
#include <string>
#include <iostream>
#include <ctime>

// ============================================================================
// Review - Customer Review Model
// OOP Concept: Encapsulation - private review data with controlled access
// OOP Concept: Operator Overloading - << for formatted display
// ============================================================================
class Review {
private:
    std::string reviewerName;
    std::string restaurantName;
    int rating;  // 1-5
    std::string comment;
    std::string timestamp;

public:
    Review();
    Review(const std::string& reviewer, const std::string& restaurant,
           int rating, const std::string& comment);

    // Getters - Encapsulation
    std::string getReviewerName() const;
    std::string getRestaurantName() const;
    int getRating() const;
    std::string getComment() const;
    std::string getTimestamp() const;

    // Utility: returns visual star representation (e.g., ★★★☆☆)
    std::string getRatingStars() const;

    // Operator Overloading: formatted stream output
    friend std::ostream& operator<<(std::ostream& os, const Review& review);
};
