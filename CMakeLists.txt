cmake_minimum_required(VERSION 3.7...3.25)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ta-bible-2 CXX)

option(TA_BIBLE_SANITIZE "Should common sanitization flags be added" OFF)

set(INC_FILES
    "bible.hpp")

set(SRC_FILES
    "bible.cpp")

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(SRC_FILES ${SRC_FILES} "main.cpp")
endif()

add_executable(ta-bible-2 ${SRC_FILES} ${INC_FILES})

target_include_directories(ta-bible-2 PRIVATE ${PROJECT_SOURCE_DIR})

target_compile_features(ta-bible-2 PRIVATE cxx_std_17)

if(TA_BIBLE_SANITIZE)
    target_compile_options(ta-bible-2 PRIVATE "-fsanitize=address,undefined")
    target_link_options(ta-bible-2 PRIVATE "-fsanitize=address,undefined")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    target_compile_options(ta-bible-2 PRIVATE "-fexceptions" "--preload-file=../kjv.txt@kjv.txt")
    target_link_options(ta-bible-2 PRIVATE "-fexceptions" "-sALLOW_MEMORY_GROWTH" "-lembind" "--preload-file=../kjv.txt@kjv.txt")
    configure_file("${PROJECT_SOURCE_DIR}/index.html" "${PROJECT_BINARY_DIR}/index.html" COPYONLY)
    configure_file("${PROJECT_SOURCE_DIR}/quote_worker.js" "${PROJECT_BINARY_DIR}/quote_worker.js" COPYONLY)
endif()
