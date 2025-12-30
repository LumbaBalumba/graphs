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

file(GLOB NODE_CONTAINERS_SOURCES src/node_containers/*.cpp)
add_library(node_containers ${NODE_CONTAINERS_SOURCES})
link_libraries(node_containers)
