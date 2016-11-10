if(NOT EXISTS ${PROJECT_BINARY_DIR}/run/luigi)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/run/luigi)
endif()

file(GLOB resources RELATIVE ${PROJECT_SOURCE_DIR}/run/luigi *.py)
foreach(resource ${resources})
    configure_file(${resource} ${PROJECT_BINARY_DIR}/run/luigi/${resource})
endforeach()

# Uncomment to enable task recording by luigi using the central service
configure_file(client.cfg ${PROJECT_BINARY_DIR}/client.cfg)
configure_file(logging.cfg ${PROJECT_BINARY_DIR}/logging.cfg)

add_subdirectory(contrib)