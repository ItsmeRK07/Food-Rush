#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>

// DSA: Graph data structure with Dijkstra's Shortest Path Algorithm
// Encapsulation: Graph representation hidden behind clean interface
class RouteManager {
private:
    // Adjacency list representation of the city map graph
    // Key: location name, Value: vector of (neighbor, distance) pairs
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> cityMap;
    static constexpr double AVG_SPEED_KMPH = 25.0;

public:
    void addRoad(const std::string& locationA, const std::string& locationB, double distance);
    double findShortestDistance(const std::string& start, const std::string& destination);
    std::vector<std::string> findShortestPath(const std::string& start, const std::string& destination);
    int estimateDeliveryTime(double distanceKm) const;
    void displayRoute(const std::string& start, const std::string& destination);
};