# Demand cmake >2.8.
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(docs)

# Create target for documentation building.
add_custom_target(doc_doxygen doxygen g4sipm.doxyfile WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

# Create target for documentation building.
add_custom_target(docs sphinx-build -b html ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} DEPENDS doc_doxygen WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
