cmake_minimum_required(VERSION 3.7...3.25)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ta-bible-2 CXX)

add_executable(ta-bible-2 "bible.cpp")

target_include_directories(ta-bible-2 PRIVATE "${PROJECT_SOURCE_DIR}")

target_compile_features(ta-bible-2 PRIVATE cxx_std_17)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    target_compile_options(ta-bible-2 PRIVATE "-fwasm-exceptions" "--preload-file=../kjv.txt@kjv.txt")
    target_link_options(ta-bible-2 PRIVATE "-fwasm-exceptions" "-sALLOW_MEMORY_GROWTH" "-lembind" "--preload-file=../kjv.txt@kjv.txt")
    configure_file("${PROJECT_SOURCE_DIR}/index.html" "${PROJECT_BINARY_DIR}/index.html" COPYONLY)
endif()
