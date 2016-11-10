if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots/root)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots/root)
endif()

file(GLOB plots RELATIVE ${PROJECT_SOURCE_DIR}/plots/root/ *.py)
foreach(plot ${plots})
    configure_file(${plot} ${PROJECT_BINARY_DIR}/plots/root/${plot})
endforeach()
