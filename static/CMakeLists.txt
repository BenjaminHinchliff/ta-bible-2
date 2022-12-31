configure_file("${CMAKE_CURRENT_LIST_DIR}/kjv.txt" "${PROJECT_BINARY_DIR}/kjv.txt" COPYONLY)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    configure_file("${CMAKE_CURRENT_LIST_DIR}/index.html" "${PROJECT_BINARY_DIR}/index.html" COPYONLY)
    configure_file("${CMAKE_CURRENT_LIST_DIR}/quote_worker.js" "${PROJECT_BINARY_DIR}/quote_worker.js" COPYONLY)
endif()
