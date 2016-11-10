# Demand cmake >2.8.
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(sample)

# Set include paths.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Require some extra libraries.
set(LIBS ${LIBS} g4sipm )

# Locate sources and headers.
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Locate rnd sources and headers
file(GLOB sources_rnd ${PROJECT_SOURCE_DIR}/src/rnd/*.cc)
set(sources ${sources} ${sources_rnd})
file(GLOB headers_rnd ${PROJECT_SOURCE_DIR}/include/rnd/*.hh)
set(headers ${headers} ${headers_rnd})

# Add persistency sources and headers.
set(sources ${sources} "${PROJECT_SOURCE_DIR}/src/persistency/PersistencyHandler.cc" 
    "${PROJECT_SOURCE_DIR}/src/persistency/PersistencyHandlerMessenger.cc" 
    "${PROJECT_SOURCE_DIR}/src/persistency/PersistVisitor.cc" 
    "${PROJECT_SOURCE_DIR}/src/persistency/PersistVisitorFactory.cc" 
    "${PROJECT_SOURCE_DIR}/src/persistency/JsonPersistVisitor.cc")
set(headers ${headers} "${PROJECT_SOURCE_DIR}/include/persistency/PersistencyHandler.hh" 
    "${PROJECT_SOURCE_DIR}/include/persistency/PersistencyHandlerMessenger.hh" 
    "${PROJECT_SOURCE_DIR}/include/persistency/PersistVisitor.hh" 
    "${PROJECT_SOURCE_DIR}/include/persistency/PersistVisitorFactory.hh" 
    "${PROJECT_SOURCE_DIR}/include/persistency/JsonPersistVisitor.hh")
    
if(WITH_ROOT)
    set(sources ${sources} "${PROJECT_SOURCE_DIR}/src/persistency/RootPersistVisitor.cc")
    set(headers ${headers} "${PROJECT_SOURCE_DIR}/include/persistency/RootPersistVisitor.hh")
	set(CMAKE_CXX_FLAGS "-DWITH_ROOT ${CMAKE_CXX_FLAGS}")
endif()    

if(WITH_SQLITE)
    set(sources ${sources} "${PROJECT_SOURCE_DIR}/src/persistency/SqlitePersistVisitor.cc")
    set(headers ${headers} "${PROJECT_SOURCE_DIR}/include/persistency/SqlitePersistVisitor.hh")
	set(CMAKE_CXX_FLAGS "-DWITH_SQLITE ${CMAKE_CXX_FLAGS}")
endif()

# Add library
set(LIB_NAME g4sipm_sample)
add_library(${LIB_NAME} ${LIBRARY_MODE} ${sources})
target_link_libraries(${LIB_NAME} ${LIBS})

# Create executable.
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cc ${headers})
target_link_libraries(${PROJECT_NAME} ${LIBS} ${LIB_NAME})

# Copy macro files.
add_subdirectory(run)
add_subdirectory(plots)
add_subdirectory(resources)

file(GLOB macros ${PROJECT_SOURCE_DIR}/*.mac)
foreach(macro ${macros})
    get_filename_component(macro-name ${macro} NAME)
    configure_file(${macro} ${PROJECT_BINARY_DIR}/${macro-name} COPYONLY)
endforeach()

# Copy sample properties files.
file(GLOB props ${PROJECT_SOURCE_DIR}/resources/*)
foreach(prop ${props})
    get_filename_component(prop-name ${prop} NAME)
    configure_file(${prop} ${PROJECT_BINARY_DIR}/resources/${prop-name} COPYONLY)
endforeach()

# Create results directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/results)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/results)
endif()

if(COMMAND add_gtest_target)
    # Create test cases.
    add_gtest_target("${LIB_NAME};${LIBS}")
    
    # ... and the coverage analysis.
    if(WITH_COVERAGE_ANALYSIS)
        # Export as XML (Cobertura format).
        add_custom_command(TARGET runSampleTest POST_BUILD
            COMMAND gcovr -x -r ${PROJECT_SOURCE_DIR} -e ".*gtest.*" >  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-test-coverage.xml
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/..
            )
        # Export as text to the console.
        add_custom_command(TARGET runSampleTest POST_BUILD
            COMMAND gcovr -r ${PROJECT_SOURCE_DIR} -e ".*gtest.*"
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/..
            )
    endif()
endif()
