if(NOT EXISTS ${PROJECT_BINARY_DIR}/resources)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/resources)
endif()

file(GLOB resources RELATIVE ${PROJECT_SOURCE_DIR}/resources *)
foreach(resource ${resources})
    configure_file(${resource} ${PROJECT_BINARY_DIR}/resources/${resource})
endforeach()
