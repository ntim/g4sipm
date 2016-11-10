# Demand cmake >2.6.
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(docs)

# Copy doxygen file.
configure_file(g4sipm.doxyfile ${PROJECT_BINARY_DIR}/g4sipm.doxyfile)

# Read contents of the README, LICENSE and CHANGELOG and compile it into the dox file.
#file(READ README.txt FILE_README)
#file(READ LICENSE.txt FILE_LICENSE)
#file(READ CHANGELOG.txt FILE_CHANGELOG)
#file(READ sample/README.txt FILE_README_SAMPLE)
#configure_file(g4sipm.dox ${PROJECT_BINARY_DIR}/g4sipm.dox)
#configure_file(g4sipm/resources/logo.png ${PROJECT_BINARY_DIR}/doc/html/logo.png COPYONLY)

# Create target for documentation building.
add_custom_target(doc_doxygen doxygen g4sipm.doxyfile)

# Create target for documentation building.
add_custom_target(docs sphinx-build -b html ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} DEPENDS doc_doxygen)
