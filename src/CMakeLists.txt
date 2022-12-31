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
    target_compile_options(ta-bible-2 PRIVATE "-fexceptions" "--preload-file=kjv.txt")
    target_link_options(ta-bible-2 PRIVATE "-fexceptions" "-sALLOW_MEMORY_GROWTH" "-lembind" "--preload-file=kjv.txt")
endif()
