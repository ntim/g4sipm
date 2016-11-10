if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots)
endif()

add_subdirectory(json)
add_subdirectory(root)
add_subdirectory(sqlite)
add_subdirectory(luigi)