# 🍕 FoodRush — Food Delivery Backend System

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus)
![OOP](https://img.shields.io/badge/Design-OOP-green?style=flat-square)
![DSA](https://img.shields.io/badge/DSA-Dijkstra's%20Algorithm-orange?style=flat-square)
![CMake](https://img.shields.io/badge/Build-CMake-red?style=flat-square&logo=cmake)

> A **terminal-based food delivery simulation** built in modern C++17 — designed to demonstrate core **Object-Oriented Programming** and **Data Structures & Algorithms** concepts through a real-world application.

FoodRush simulates a campus food delivery ecosystem where users can browse restaurants, order food, track deliveries routed via shortest-path algorithms, and manage a virtual wallet — all from an interactive, emoji-rich command-line interface.

---

## ✨ Features

- ✅ Interactive CLI with role-based authentication (register / login)
- ✅ Multiple restaurants with rich, browsable menus
- ✅ Smart cart system (single-restaurant validation)
- ✅ Order processing with tax (5%) and distance-based delivery fees
- ✅ **Dijkstra's shortest path** for delivery routing on a campus graph
- ✅ Delivery time estimation based on distance
- ✅ Automatic delivery partner assignment & rating
- ✅ Complete order history and real-time tracking
- ✅ Restaurant ratings and user reviews
- ✅ Search and filter (by name, veg/non-veg, price sort)
- ✅ Virtual wallet system with persistent balance
- ✅ File-based data persistence (users, orders, reviews)
- ✅ Robust input validation and error handling

---

## 🎓 OOP Concepts Demonstrated

| Concept | Implementation |
|---|---|
| **Abstraction** | `User` abstract base class with pure virtual `displayDashboard()` |
| **Inheritance** | `Customer` and `DeliveryPartner` inherit from `User` |
| **Polymorphism** | Virtual function overrides, `operator<<` overloading |
| **Encapsulation** | Private data members with public getters/setters, validation in setters |
| **Composition** | `Restaurant` has `FoodItem`s and `Review`s, `Order` has `FoodItem`s |
| **Operator Overloading** | `<<`, `==` operators on multiple classes |
| **Smart Pointers** | `unique_ptr` for session management (RAII) |
| **Static Members** | `InputValidator`, `Display` utility classes, `Order::nextOrderId` |
| **Enum Classes** | `OrderStatus` for type-safe status tracking |

---

## 📊 DSA Concepts

| Data Structure / Algorithm | Usage |
|---|---|
| **Graph (Adjacency List)** | Campus road network representation |
| **Dijkstra's Shortest Path** | Optimal delivery route calculation |
| **Priority Queue (Min-Heap)** | Efficient shortest-distance extraction in Dijkstra's |
| **Hash Maps (`unordered_map`)** | O(1) user lookups, adjacency list storage |

---

## 📁 Project Structure

```
FoodRush/
├── main.cpp                  # Application entry point & CLI loop
├── CMakeLists.txt            # CMake build configuration
├── README.md                 # This file
├── .gitignore
│
├── models/                   # Data model classes
│   ├── User.h                # Abstract base class
│   ├── Customer.h / .cpp     # Customer (inherits User)
│   ├── DeliveryPartner.h/.cpp# Delivery partner (inherits User)
│   ├── Restaurant.h / .cpp   # Restaurant with menu & reviews
│   ├── FoodItem.h / .cpp     # Individual food item
│   ├── Cart.h / .cpp         # Shopping cart (single-restaurant)
│   ├── Order.h / .cpp        # Order with status tracking
│   └── Review.h / .cpp       # User review
│
├── services/                 # Business logic layer
│   ├── AuthService.h / .cpp  # Registration, login, wallet sync
│   ├── RouteManager.h / .cpp # Graph + Dijkstra's algorithm
│   ├── RestaurantManager.h/.cpp # Restaurant initialization & search
│   ├── DeliveryService.h/.cpp# Partner assignment & management
│   └── OrderManager.h / .cpp # Order placement, history, persistence
│
├── utils/                    # Utility helpers
│   ├── Display.h / .cpp      # Terminal UI formatting
│   └── InputValidator.h/.cpp # Type-safe input validation
│
└── data/                     # Persistent data files
    ├── users.txt              # Registered user accounts
    ├── orders.txt             # Order history
    └── reviews.txt            # Restaurant reviews
```

---

## 🔨 How to Build & Run

### Prerequisites

- A C++17 compatible compiler (g++ 7+, MSVC 2017+, Clang 5+)
- CMake 3.10+ (optional, for Method 2)

### Method 1: Direct Compilation

```bash
# Linux / macOS
g++ -std=c++17 -o FoodRush main.cpp models/*.cpp services/*.cpp utils/*.cpp
./FoodRush

# Windows (MinGW)
g++ -std=c++17 -o FoodRush.exe main.cpp models/*.cpp services/*.cpp utils/*.cpp
FoodRush.exe

# Windows (MSVC Developer Command Prompt)
cl /std:c++17 /EHsc /Fe:FoodRush.exe main.cpp models\*.cpp services\*.cpp utils\*.cpp
FoodRush.exe
```

### Method 2: Using CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .

# Run
./FoodRush        # Linux / macOS
FoodRush.exe      # Windows
```

---

## 🗺️ Campus Map

The delivery route system uses a weighted undirected graph representing the campus:

```
                    ┌──────────────┐
                    │  Driver Hub  │
                    └──────┬───────┘
                           │ 1.0 km
                    ┌──────┴───────┐
            ┌───────│   Main Gate  │───────┐
            │  1.5  └──────────────┘  2.0  │
            │                              │
     ┌──────┴───────┐              ┌───────┴──────┐
     │  Food Court  │              │Library Square │
     └──┬───────┬───┘              └──┬────────┬───┘
    0.8 │       │ 2.5             0.5 │        │ 1.5
        │       │                     │        │
  ┌─────┴────┐  │               ┌─────┴──────┐ │
  │PRP Block │  │               │  Tech Park │ │
  └─────┬────┘  │               └─────┬──────┘ │
    1.2 │       │                 1.8 │        │
        │       │                     │        │
        │  ┌────┴──────────┐    ┌─────┴────────┴──┐
    3.0 └──│ Men's Hostel  │────│ Sports Complex  │
           └───────────────┘2.0 └─────────────────┘
```

### 🏪 Restaurant Locations

| # | Restaurant | Location |
|---|---|---|
| 1 | Campus Kitchen | Food Court |
| 2 | Dragon Wok | Tech Park |
| 3 | South Express | Library Square |
| 4 | Burger Junction | Sports Complex |
| 5 | Chai & More | Main Gate |

---

## 🛠️ Tech Stack

| Technology | Purpose |
|---|---|
| **C++17** | Core language with modern features (filesystem, structured bindings) |
| **STL** | Containers (vector, unordered_map, priority_queue), algorithms |
| **File I/O** | fstream-based data persistence |
| **CMake** | Cross-platform build system |

---

## 📝 Sample Credentials

A test account is pre-loaded for quick exploration:

| Username | Password | Wallet Balance |
|---|---|---|
| `testuser` | `pass123` | ₹500.00 |

---

## 📜 License

This project is created for educational purposes to demonstrate OOP and DSA concepts in C++.

---

<p align="center">
  Made with ❤️ and C++ &nbsp;|&nbsp; <strong>FoodRush v1.0</strong>
</p>
