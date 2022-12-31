cmake_minimum_required(VERSION 3.7...3.25)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ta-bible-2 CXX)

option(TA_BIBLE_SANITIZE "Should common sanitization flags be added" OFF)

add_subdirectory("static")
add_subdirectory("src")
