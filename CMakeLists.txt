project(AdvPT)
cmake_minimum_required(VERSION 3.19)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic")

find_package(nlohmann_json 3.2.0 REQUIRED)

#target_link_libraries(factorioTest PRIVATE nlohmann_json::nlohmann_json)

add_executable(factorioTest main.cpp factorioBase.cpp)
