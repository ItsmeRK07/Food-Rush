#pragma once
#include <string>

class User {
protected:
    std::string username;
    std::string password;

public:
    User(std::string uName, std::string pass) : username(uName), password(pass) {}
    virtual ~User() = default;

    std::string getUsername() const { return username; }
    bool verifyPassword(std::string pass) const { return password == pass; }

    virtual void displayDashboard() = 0;
};