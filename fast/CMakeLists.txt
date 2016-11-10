# Demand cmake >2.8.
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(fast)

# Set include paths.
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../sample/include)

# Require some extra libraries.
set(LIBS g4sipm g4sipm_sample ${LIBS})

# Locate sources and headers.
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Create executable.
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cc ${headers} ${sources})
target_link_libraries(${PROJECT_NAME} ${LIBS} ${LIB_NAME})

file(GLOB macros ${PROJECT_SOURCE_DIR}/*.mac)
foreach(macro ${macros})
    get_filename_component(macro-name ${macro} NAME)
    configure_file(${macro} ${PROJECT_BINARY_DIR}/${macro-name} COPYONLY)
endforeach()
