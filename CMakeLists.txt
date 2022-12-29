cmake_minimum_required(VERSION 3.7...3.25)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ta-bible-2 CXX)

add_executable(ta-bible-2 "bible.cpp")

target_compile_features(ta-bible-2 PRIVATE cxx_std_20)
