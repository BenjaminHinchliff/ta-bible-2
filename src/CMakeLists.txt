set(INC_FILES_DIR "${PROJECT_SOURCE_DIR}/include/")

set(INC_FILES
    "bible.hpp"
    "trie.hpp")

list(TRANSFORM INC_FILES PREPEND ${INC_FILES_DIR})

set(SRC_FILES
    "bible.cpp"
    "trie.cpp")

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(SRC_FILES ${SRC_FILES} "main.cpp")
endif()

add_executable(ta-bible-2 ${SRC_FILES} ${INC_FILES})

target_include_directories(ta-bible-2 PRIVATE ${INC_FILES_DIR})

target_compile_features(ta-bible-2 PRIVATE cxx_std_17)

if(TA_BIBLE_SANITIZE)
    target_compile_options(ta-bible-2 PRIVATE "-fsanitize=address,undefined")
    target_link_options(ta-bible-2 PRIVATE "-fsanitize=address,undefined")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(EMSCRIPTEN_COMMON_FLAGS "-fwasm-exceptions" "--preload-file=kjv.txt")
    target_compile_options(ta-bible-2 PRIVATE ${EMSCRIPTEN_COMMON_FLAGS})
    target_link_options(ta-bible-2 PRIVATE ${EMSCRIPTEN_COMMON_FLAGS} "-sEXPORT_EXCEPTION_HANDLING_HELPERS=1" "-sALLOW_MEMORY_GROWTH=1" "-lembind")
endif()
