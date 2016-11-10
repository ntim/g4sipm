if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots/json)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots/json)
endif()

file(GLOB plots RELATIVE ${PROJECT_SOURCE_DIR}/plots/json/ *.py)
foreach(plot ${plots})
    configure_file(${plot} ${PROJECT_BINARY_DIR}/plots/json/${plot})
endforeach()
