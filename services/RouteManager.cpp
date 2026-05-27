#include "RouteManager.h"
#include <cmath>
#include <iomanip>

// DSA: Bidirectional edge insertion for undirected graph
// Adds a road (edge) between two locations with a given distance
void RouteManager::addRoad(const std::string& locationA, const std::string& locationB, double distance) {
    cityMap[locationA].push_back({locationB, distance});
    cityMap[locationB].push_back({locationA, distance});
}

// DSA: Dijkstra's Algorithm using min-heap (priority_queue)
// Time Complexity: O((V + E) log V) where V = vertices, E = edges
// Returns shortest distance between start and destination, or -1.0 if no path exists
double RouteManager::findShortestDistance(const std::string& start, const std::string& destination) {
    // Edge case: start or destination not in graph
    if (cityMap.find(start) == cityMap.end() || cityMap.find(destination) == cityMap.end()) {
        return -1.0;
    }

    // Distance map initialized to infinity
    std::unordered_map<std::string, double> dist;
    for (const auto& pair : cityMap) {
        dist[pair.first] = std::numeric_limits<double>::max();
    }
    dist[start] = 0.0;

    // Min-heap: (distance, node) — smallest distance on top
    std::priority_queue<std::pair<double, std::string>,
                        std::vector<std::pair<double, std::string>>,
                        std::greater<std::pair<double, std::string>>> pq;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        // Skip if we've already found a shorter path to this node
        if (currentDist > dist[currentNode]) continue;

        // Early termination if we've reached the destination
        if (currentNode == destination) {
            return dist[destination];
        }

        // Relaxation: explore neighbors
        for (const auto& [neighbor, weight] : cityMap[currentNode]) {
            double newDist = currentDist + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                pq.push({newDist, neighbor});
            }
        }
    }

    return (dist[destination] == std::numeric_limits<double>::max()) ? -1.0 : dist[destination];
}

// DSA: Modified Dijkstra's that tracks predecessor map for path reconstruction
// Returns vector of location names from start to destination
std::vector<std::string> RouteManager::findShortestPath(const std::string& start, const std::string& destination) {
    // Edge case: start or destination not in graph
    if (cityMap.find(start) == cityMap.end() || cityMap.find(destination) == cityMap.end()) {
        return {};
    }

    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> predecessor; // Tracks path

    for (const auto& pair : cityMap) {
        dist[pair.first] = std::numeric_limits<double>::max();
    }
    dist[start] = 0.0;

    std::priority_queue<std::pair<double, std::string>,
                        std::vector<std::pair<double, std::string>>,
                        std::greater<std::pair<double, std::string>>> pq;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        if (currentDist > dist[currentNode]) continue;

        if (currentNode == destination) break;

        for (const auto& [neighbor, weight] : cityMap[currentNode]) {
            double newDist = currentDist + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                predecessor[neighbor] = currentNode;
                pq.push({newDist, neighbor});
            }
        }
    }

    // No path found
    if (dist[destination] == std::numeric_limits<double>::max()) {
        return {};
    }

    // Backtrack from destination to start using predecessor map
    std::vector<std::string> path;
    std::string current = destination;
    while (current != start) {
        path.push_back(current);
        current = predecessor[current];
    }
    path.push_back(start);

    // Reverse to get start -> destination order
    std::reverse(path.begin(), path.end());
    return path;
}

// Estimates delivery time in minutes based on distance
// Includes 10 minutes preparation time, minimum 15 minutes total
int RouteManager::estimateDeliveryTime(double distanceKm) const {
    int travelTime = (int)std::ceil((distanceKm / AVG_SPEED_KMPH) * 60.0);
    int totalTime = travelTime + 10; // Add 10 minutes for preparation
    return std::max(totalTime, 15);  // Minimum 15 minutes
}

// Displays the route, distance, and estimated delivery time
void RouteManager::displayRoute(const std::string& start, const std::string& destination) {
    std::vector<std::string> path = findShortestPath(start, destination);
    double distance = findShortestDistance(start, destination);

    if (path.empty() || distance < 0) {
        std::cout << "\n  \u274C No route available from " << start << " to " << destination << std::endl;
        return;
    }

    // Display route path
    std::cout << "\n  \U0001F5FA  Route: ";
    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " \u2192 ";
        }
    }
    std::cout << std::endl;

    // Display distance and estimated time
    std::cout << "  \U0001F4CF Distance: " << std::fixed << std::setprecision(1) << distance << " km" << std::endl;
    std::cout << "  \u23F1  Estimated Time: " << estimateDeliveryTime(distance) << " minutes" << std::endl;
}
