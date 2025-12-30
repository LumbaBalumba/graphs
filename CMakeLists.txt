cmake_minimum_required(VERSION 3.22)
project(aim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Release)

include_directories(graphs INTERFACE include)

find_package(Boost REQUIRED)

file(GLOB NODE_CONTAINERS_SOURCES src/node_containers/*.cpp)
add_library(node_containers ${NODE_CONTAINERS_SOURCES})
target_include_directories(node_containers PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(node_containers PRIVATE ${Boost_LIBRARIES})

add_library(graphs INTERFACE)
