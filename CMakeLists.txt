# Demand cmake >2.8.
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(G4Sipm)

# Include scripts.
include(CMakeMacros.cmake)

# Coverage analysis.
# Setting this to true requires a "make clean".
option(WITH_COVERAGE_ANALYSIS "Enable the coverage analysis." OFF)
if(WITH_COVERAGE_ANALYSIS)
	set(JANSSON_COVERAGE 1 CACHE BOOL "We have to enable coverage analysis for Jansson as well" FORCE)
    # Add definitions for code coverage.
    add_definitions("-fprofile-arcs -ftest-coverage -g")
    # Add coverage library to EXTRA_LIBS.
    set(LIBS ${LIBS} gcov)
endif()

# Boost
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.50.0 COMPONENTS date_time program_options filesystem system regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

# Add Jansson
set(BUILD_DOCS 0 CACHE BOOL "Build documentation (uses python-sphinx)." FORCE)
set(WITHOUT_TESTS 1 CACHE BOOL "Don't build tests ('make test' to execute tests)" FORCE)
set(JANSSON_BUILD_DOCS 0 CACHE BOOL "Don't build documentation" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/externals/jansson")
include_directories("${PROJECT_SOURCE_DIR}/externals/jansson/src")
include_directories("${PROJECT_BINARY_DIR}/externals/jansson/include")
set(LIBS ${LIBS} jansson)

# Find Geant4 package, activating all available UI and Vis drivers by default
option(WITH_GEANT4_UIVIS "Build with all Geant4 UIs and all visualization drivers" ON)
option(WITH_GEANT4_OPENGL "Build with Geant4 shell UI and OpenGl visualization" OFF)
option(WITH_GEANT4_QT "Build with Geant4 Qt UI" OFF)

if(WITH_GEANT4_OPENGL)
	find_package(Geant4 REQUIRED vis_opengl_x11 ui_tcsh)
elseif(WITH_GEANT4_QT)
	find_package(Geant4 REQUIRED qt)
elseif(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

add_definitions(${Geant4_DEFINITIONS})
include(${Geant4_USE_FILE})
set(LIBS ${LIBS} ${Geant4_LIBRARIES})

# Switch for static / shared library building.
if(Geant4_shared_FOUND)
	set(LIBRARY_MODE "SHARED")
else()
	set(LIBRARY_MODE "STATIC")
endif()

option(WITH_ROOT "Enable ROOT." ON)
if(WITH_ROOT)
	execute_process(COMMAND root-config --libs OUTPUT_VARIABLE root_libs OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE root_cflags OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND root-config --incdir OUTPUT_VARIABLE root_includes OUTPUT_STRIP_TRAILING_WHITESPACE)
	add_definitions(${root_cflags})
	include_directories(${root_includes})
	set(LIBS ${LIBS} ${root_libs})
	set(CMAKE_CXX_FLAGS "-DWITH_ROOT ${CMAKE_CXX_FLAGS}")
endif()

option(WITH_SQLITE "Enable SQlite support." ON)
if(WITH_SQLITE)
	set(LIBS ${LIBS} sqlite3)
	set(CMAKE_CXX_FLAGS "-DWITH_SQLITE ${CMAKE_CXX_FLAGS}")
endif()

# Add subdirectories
add_subdirectory(externals/googletest/googletest)
include_directories(externals/googletest/googletest/include)
add_subdirectory(g4sipm)
include_directories(g4sipm/include)
add_subdirectory(sample)
add_subdirectory(fast)
add_subdirectory(docs)
