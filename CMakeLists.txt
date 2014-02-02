cmake_minimum_required(VERSION 2.8.9)

project(qt5helper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)

add_definitions(-std=c++11)

find_package(Qt5Widgets)
add_library(qt5helper STATIC enchanthighlighter.cc)
qt5_use_modules(qt5helper Widgets)

find_package(Enchant)
include_directories(${ENCHANT_INCLUDE_DIRS})
target_link_libraries (qt5helper ${ENCHANT_LIBRARIES})


