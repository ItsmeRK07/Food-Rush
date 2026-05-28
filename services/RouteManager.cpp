#include "RouteManager.h"
#include "../utils/FoodRushException.h"
#include <cmath>
#include <iomanip>

void RouteManager::addRoad(const std::string& locationA, const std::string& locationB, double distance) {
    cityMap[locationA].push_back(std::make_pair(locationB, distance));
    cityMap[locationB].push_back(std::make_pair(locationA, distance));
}

void RouteManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string locationA, locationB, distStr;
        std::getline(iss, locationA, ',');
        std::getline(iss, locationB, ',');
        std::getline(iss, distStr);
        try {
            double distance = std::stod(distStr);
            addRoad(locationA, locationB, distance);
        } catch (...) { continue; }
    }
    file.close();
}

bool RouteManager::hasLocations() const { return !cityMap.empty(); }

double RouteManager::findShortestDistance(const std::string& start, const std::string& destination) {
    if (cityMap.find(start) == cityMap.end() || cityMap.find(destination) == cityMap.end()) {
        throw RouteException("Location not found in campus map: " +
            (cityMap.find(start) == cityMap.end() ? start : destination));
    }

    std::unordered_map<std::string, double> dist;
    for (auto it = cityMap.begin(); it != cityMap.end(); ++it) {
        dist[it->first] = std::numeric_limits<double>::max();
    }
    dist[start] = 0.0;

    typedef std::pair<double, std::string> PQEntry;
    std::priority_queue<PQEntry, std::vector<PQEntry>, std::greater<PQEntry>> pq;
    pq.push(std::make_pair(0.0, start));

    while (!pq.empty()) {
        double currentDist = pq.top().first;
        std::string currentNode = pq.top().second;
        pq.pop();

        if (currentDist > dist[currentNode]) continue;
        if (currentNode == destination) return dist[destination];

        const auto& neighbors = cityMap[currentNode];
        for (size_t i = 0; i < neighbors.size(); ++i) {
            const std::string& neighbor = neighbors[i].first;
            double weight = neighbors[i].second;
            double newDist = currentDist + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                pq.push(std::make_pair(newDist, neighbor));
            }
        }
    }

    if (dist[destination] == std::numeric_limits<double>::max()) {
        throw RouteException("No route available from " + start + " to " + destination);
    }
    return dist[destination];
}

std::vector<std::string> RouteManager::findShortestPath(const std::string& start, const std::string& destination) {
    if (cityMap.find(start) == cityMap.end() || cityMap.find(destination) == cityMap.end()) {
        throw RouteException("Location not found in campus map: " +
            (cityMap.find(start) == cityMap.end() ? start : destination));
    }

    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> predecessor;
    for (auto it = cityMap.begin(); it != cityMap.end(); ++it) {
        dist[it->first] = std::numeric_limits<double>::max();
    }
    dist[start] = 0.0;

    typedef std::pair<double, std::string> PQEntry;
    std::priority_queue<PQEntry, std::vector<PQEntry>, std::greater<PQEntry>> pq;
    pq.push(std::make_pair(0.0, start));

    while (!pq.empty()) {
        double currentDist = pq.top().first;
        std::string currentNode = pq.top().second;
        pq.pop();

        if (currentDist > dist[currentNode]) continue;
        if (currentNode == destination) break;

        const auto& neighbors = cityMap[currentNode];
        for (size_t i = 0; i < neighbors.size(); ++i) {
            const std::string& neighbor = neighbors[i].first;
            double weight = neighbors[i].second;
            double newDist = currentDist + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                predecessor[neighbor] = currentNode;
                pq.push(std::make_pair(newDist, neighbor));
            }
        }
    }

    if (dist[destination] == std::numeric_limits<double>::max()) {
        throw RouteException("No route available from " + start + " to " + destination);
    }

    std::vector<std::string> path;
    std::string current = destination;
    while (current != start) {
        path.push_back(current);
        current = predecessor[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

int RouteManager::estimateDeliveryTime(double distanceKm) const {
    int travelTime = (int)std::ceil((distanceKm / AVG_SPEED_KMPH) * 60.0);
    int totalTime = travelTime + 10;
    return std::max(totalTime, 15);
}

void RouteManager::displayRoute(const std::string& start, const std::string& destination) {
    try {
        std::vector<std::string> path = findShortestPath(start, destination);
        double distance = findShortestDistance(start, destination);

        std::cout << "\n  \xF0\x9F\x97\xBA  Route: ";
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i];
            if (i < path.size() - 1) std::cout << " \xE2\x86\x92 ";
        }
        std::cout << std::endl;
        std::cout << "  \xF0\x9F\x93\x8F Distance: " << std::fixed << std::setprecision(1) << distance << " km" << std::endl;
        std::cout << "  \xE2\x8F\xB1  Estimated Time: " << estimateDeliveryTime(distance) << " minutes" << std::endl;
    } catch (const RouteException& e) {
        std::cout << "\n  \xE2\x9D\x8C " << e.what() << std::endl;
    }
}
