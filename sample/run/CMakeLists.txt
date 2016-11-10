if(NOT EXISTS ${PROJECT_BINARY_DIR}/run)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/run)
endif()

file(GLOB resources RELATIVE ${PROJECT_SOURCE_DIR}/run *.py)
foreach(resource ${resources})
    configure_file(${resource} ${PROJECT_BINARY_DIR}/run/${resource})
endforeach()

add_subdirectory(luigi)