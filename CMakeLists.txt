cmake_minimum_required(VERSION 3.10)
project(FoodRush VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files
file(GLOB MODEL_SOURCES "models/*.cpp")
file(GLOB SERVICE_SOURCES "services/*.cpp")
file(GLOB UTIL_SOURCES "utils/*.cpp")

add_executable(FoodRush
    main.cpp
    ${MODEL_SOURCES}
    ${SERVICE_SOURCES}
    ${UTIL_SOURCES}
)

target_include_directories(FoodRush PRIVATE ${CMAKE_SOURCE_DIR})
