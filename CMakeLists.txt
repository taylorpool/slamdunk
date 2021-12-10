cmake_minimum_required(VERSION 3.15)
project(SLAMDunk)
include(FetchContent)

find_package(Ceres CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

add_library(
    SLAMDunk
    src/slamdunk.cpp
)

target_include_directories(
    SLAMDunk
    PUBLIC
    "include"
)

target_link_libraries(
    SLAMDunk
    PRIVATE
    ceres
    Ceres::ceres
)

add_library(
    BundleAdjustment
    INTERFACE
)

target_include_directories(
    BundleAdjustment
    INTERFACE
    "include"
)

target_link_libraries(
    BundleAdjustment
    INTERFACE
    ceres
    Ceres::ceres
)

add_executable(
    tumBundle
    src/tum_bundle_adjustment.cpp
)

target_link_libraries(
    tumBundle
    PUBLIC
    BundleAdjustment
    ceres
    Ceres::ceres
)

target_include_directories(
    tumBundle
    PUBLIC
    "include"
)

option(MAKE_TESTS "Make tests" ON)

if(${MAKE_TESTS})

    add_executable(
        demo_bundle
        test/demo_bundle_adjustment.cpp
    )

    target_link_libraries(
        demo_bundle
        BundleAdjustment
    )

    add_executable(
        ceres_tests
        test/test_ceres.cpp
    )

    target_link_libraries(
        ceres_tests
        gtest_main
        ceres
        Ceres::ceres
    )

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(
        tests
        test/test_slamdunk.cpp
    )
    
    target_link_libraries(
        tests 
        gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(tests)

endif()