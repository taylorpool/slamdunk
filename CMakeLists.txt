cmake_minimum_required(VERSION 3.15)
project(SLAMDunk)
include(FetchContent)

add_subdirectory(cmake)

FetchContent_MakeAvailable(ceres-solver)

option(MAKE_TESTS "Make tests" ON)

if(${MAKE_TESTS})
    FetchContent_MakeAvailable(googletest)
    enable_testing()

    add_executable(
        tests
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_slamdunk.cpp")

    target_link_libraries(
        tests 
        PUBLIC
        Ceres
        gtest_main)

    include(GoogleTest)
    gtest_discover_tests(tests)

endif()