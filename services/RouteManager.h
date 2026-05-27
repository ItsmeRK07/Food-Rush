#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <limits>

class RouteManager {
private:
    // The Graph: Node -> vector of {Neighbor Node, Distance}
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> cityMap;

public:
    // Add a two-way road between two locations
    void addRoad(std::string locationA, std::string locationB, double distance) {
        cityMap[locationA].push_back({locationB, distance});
        cityMap[locationB].push_back({locationA, distance}); 
    }

    // Dijkstra's Algorithm to find the shortest distance
    double findShortestDistance(std::string start, std::string destination) {
        // Priority queue stores {distance, nodeName} and sorts by smallest distance first
        std::priority_queue<std::pair<double, std::string>, 
                            std::vector<std::pair<double, std::string>>, 
                            std::greater<std::pair<double, std::string>>> pq;
        
        // Map to keep track of the shortest distance to each node
        std::unordered_map<std::string, double> distances;
        
        // Initialize all known locations to "infinity"
        for (const auto& pair : cityMap) {
            distances[pair.first] = std::numeric_limits<double>::infinity();
        }
        
        // Starting point distance is 0
        distances[start] = 0.0;
        pq.push({0.0, start});

        while (!pq.empty()) {
            double currentDist = pq.top().first;
            std::string currentNode = pq.top().second;
            pq.pop();

            // If we reached our target, we can stop searching!
            if (currentNode == destination) return currentDist;

            // If we pulled a stale, longer distance from the queue, ignore it
            if (currentDist > distances[currentNode]) continue;

            // Look at all connected roads
            for (const auto& neighbor : cityMap[currentNode]) {
                std::string nextNode = neighbor.first;
                double edgeWeight = neighbor.second;

                // If traveling through currentNode is shorter than the old path...
                if (distances[currentNode] + edgeWeight < distances[nextNode]) {
                    distances[nextNode] = distances[currentNode] + edgeWeight;
                    pq.push({distances[nextNode], nextNode});
                }
            }
        }
        
        return -1.0; // Return -1 if there is no path between the two locations
    }
};