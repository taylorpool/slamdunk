cmake_minimum_required(VERSION 3.15)
project(SLAMDunk)

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_library(
    SLAMDunk
    src/load_data.cpp
)

target_include_directories(
    SLAMDunk
    PUBLIC
    "include"
    "${OpenCV_INCLUDE_DIRS}"
    "${Ceres_INCLUDE_DIRS}"
)

target_link_libraries(
    SLAMDunk
    PUBLIC
    "${OpenCV_LIBS}"
    "${Ceres_LIBS}"
)

add_executable(
    SLAMBAM
    src/slamdunk.cpp
)

target_include_directories(
    SLAMBAM
    PUBLIC
    "include"
    "${OpenCV_INCLUDE_DIRS}"
)

target_link_libraries(
    SLAMBAM
    PUBLIC 
    SLAMDunk
)



option(MAKE_TESTS "Make Tests" ON)

if(${MAKE_TESTS})
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(
        ceres_test
        test/test_ceres.cpp
    )
    target_link_libraries(ceres_test gtest_main)

    include(GoogleTest)
    gtest_discover_tests(ceres_test)

endif()