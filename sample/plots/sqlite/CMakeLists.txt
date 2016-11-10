if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots/sqlite)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots/sqlite)
endif()

file(GLOB plots RELATIVE ${PROJECT_SOURCE_DIR}/plots/sqlite/ *.py)
foreach(plot ${plots})
    configure_file(${plot} ${PROJECT_BINARY_DIR}/plots/sqlite/${plot})
endforeach()
