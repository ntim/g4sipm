if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots/luigi)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots/luigi)
endif()

file(GLOB plots RELATIVE ${PROJECT_SOURCE_DIR}/plots/luigi/ *.py)
foreach(plot ${plots})
    configure_file(${plot} ${PROJECT_BINARY_DIR}/plots/luigi/${plot})
endforeach()

add_subdirectory(analysis)
add_subdirectory(contrib)