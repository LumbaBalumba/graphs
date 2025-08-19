cmake_minimum_required(VERSION 3.22)
project(aim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Release)

include_directories(task PRIVATE include)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

add_library(graph src/node_graph.cpp)
link_libraries(graph)

file(GLOB NODE_CONTAINERS_SOURCES src/node_containers/*.cpp)
add_library(node_containers ${NODE_CONTAINERS_SOURCES})
link_libraries(node_containers)

add_library(node_graph_processor src/graph_processor.cpp)
link_libraries(node_graph_processor)

add_executable(main src/main.cpp)
