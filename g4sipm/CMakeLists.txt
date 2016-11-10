# Demand cmake >2.8.
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(g4sipm)

# Compiler definitions
add_definitions("-Wall -pedantic")

# Add boost regex (needed for the properties file parsing).
set(LIBS ${LIBS})

# Set include paths.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers.
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Create library.
add_library(${PROJECT_NAME} ${LIBRARY_MODE} ${sources})

# Add link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy resources.
file(GLOB_RECURSE resources ${PROJECT_SOURCE_DIR}/resources/*.*)
foreach(resource ${resources})
    get_filename_component(resource-name ${resource} NAME)
    configure_file(${resource} ${PROJECT_BINARY_DIR}/resources/${resource-name} COPYONLY)
endforeach()

if(COMMAND add_gtest_target)
    # Create test cases.
    add_gtest_target("${PROJECT_NAME};${LIBS}")
    
    # ... and the coverage analysis.
    if(WITH_COVERAGE_ANALYSIS)
        # Export as XML (Cobertura format).
        add_custom_command(TARGET runG4sipmTest POST_BUILD
            COMMAND gcovr -x -r ${PROJECT_SOURCE_DIR} -e ".*gtest.*" >  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-test-coverage.xml
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/..
            )
        # Export as text to the console.
        add_custom_command(TARGET runG4sipmTest POST_BUILD
            COMMAND gcovr -r ${PROJECT_SOURCE_DIR} -e ".*gtest.*"
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/..
            )
    endif()
endif()
