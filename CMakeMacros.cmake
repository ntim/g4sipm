#
# Creates test cases for the current project usign gtest and header files 
# in a 'test' subdirectory.
#
macro(add_gtest_target libs)
    include_directories(${PROJECT_SOURCE_DIR}/test/)
    file(GLOB testHeaders ${PROJECT_SOURCE_DIR}/test/*.hh)
    add_executable(${PROJECT_NAME}Test test/test.cc ${testHeaders})
    target_link_libraries(${PROJECT_NAME}Test ${libs} gtest gtest_main)
    # Capitalize first letter (compatible with cmake 2.6)
    set(name-uppercase "${PROJECT_NAME}Test")
    # Get first letter and capitalize.
    string(SUBSTRING ${name-uppercase} 0 1 first-letter)
    string(TOUPPER ${first-letter} first-letter)
    # Get the rest of the macro name.
    string(LENGTH ${name-uppercase} length)
    # Substract one from the length.
    execute_process(COMMAND expr ${length} - 1 OUTPUT_VARIABLE length)
    string(SUBSTRING ${name-uppercase} 1 ${length} rest)
    # Compose macro-name with first letter uppercase.
    set(name-uppercase "${first-letter}${rest}")
    # Create custom make target to execute the test.
    add_custom_target(run${name-uppercase} ./${PROJECT_NAME}Test 
        --gtest_output="xml:${PROJECT_NAME}-test-report.xml"
        DEPENDS ${PROJECT_NAME}Test)
endmacro()
