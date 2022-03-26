cmake_minimum_required(VERSION 3.5)

project(myliba LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(myliba SHARED
  myliba_global.h
  myliba.cpp
  myliba.h
)

target_link_libraries(myliba PRIVATE Qt5::Core)

target_compile_definitions(myliba PRIVATE MYLIBA_LIBRARY)
