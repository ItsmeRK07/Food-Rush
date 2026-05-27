# FoodRush - Food Delivery Backend System

FoodRush is a terminal-based backend simulation of a food delivery platform (like Swiggy, Zomato, or UberEats). Built with Modern C++, this project demonstrates a strong understanding of Object-Oriented Programming (OOP), Data Structures & Algorithms (DSA), and clean architecture.

Instead of just performing basic CRUD operations, FoodRush implements real-world business logic, including a virtual wallet system, cart validation, and graph-based delivery routing.

## Core Features
* **Role-Based Authentication:** Secure login and registration system for Customers using file-based persistence (`<fstream>`) and Smart Pointers (`std::unique_ptr`) for memory safety.
* **Smart Cart System:** Enforces real-world business logic (e.g., users can only add items from one restaurant at a time).
* **Dynamic Order Processing:** Calculates subtotal, applies taxes, and deducts funds directly from the user's virtual wallet.
* **Algorithmic Routing (DSA):** Uses Dijkstra's Shortest Path Algorithm on a weighted graph to find the fastest route between the restaurant and the customer, dynamically calculating delivery fees based on distance.

## Tech Stack & Concepts
* **Language:** C++ (C++14/17)
* **Architecture:** Service-Oriented Architecture (Models vs. Services)
* **Memory Management:** Smart Pointers (`<memory>`)
* **DSA Applied:** Graphs, Adjacency Lists, Priority Queues (Min-Heap)
* **OOP Principles:** Encapsulation, Inheritance, Polymorphism, Abstraction, and Composition.

## Project Structure
```text
FoodRush/
│
├── models/               # Data structures and entities
│   ├── User.h
│   ├── Customer.h
│   ├── DeliveryPartner.h
│   ├── FoodItem.h
│   ├── Restaurant.h
│   ├── Cart.h
│   └── Order.h
│
├── services/             # Core business logic and algorithms
│   ├── AuthService.h     # Handles file I/O and user sessions
│   └── RouteManager.h    # Graph implementation and Dijkstra's algorithm
│
├── users.txt             # Text-based database for persistence
└── main.cpp              # Application entry point
