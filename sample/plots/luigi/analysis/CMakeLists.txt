if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots/sqlite)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots/luigi/analysis)
endif()

file(GLOB plots RELATIVE ${PROJECT_SOURCE_DIR}/plots/luigi/analysis/ *.py)
foreach(plot ${plots})
    configure_file(${plot} ${PROJECT_BINARY_DIR}/plots/luigi/analysis/${plot})
endforeach()
