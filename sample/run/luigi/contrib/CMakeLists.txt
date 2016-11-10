if(NOT EXISTS ${PROJECT_BINARY_DIR}/run/luigi/contrib)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/run/luigi/contrib)
endif()

file(GLOB resources RELATIVE ${PROJECT_SOURCE_DIR}/run/luigi/contrib *.py)
foreach(resource ${resources})
    configure_file(${resource} ${PROJECT_BINARY_DIR}/run/luigi/contrib/${resource})
endforeach()
