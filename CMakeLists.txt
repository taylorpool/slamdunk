cmake_minimum_required(VERSION 3.15)
project(SLAMDunk)

find_package(Eigen REQUIRED)
find_package(glog REQUIRED)
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

add_library(SLAMDunk
# Put source files here
)

target_include_directories(
    SLAMDunk
    PUBLIC
    "include"
)

target_link_libraries(
    SLAMDunk
    PUBLIC
    Ceres::ceres
    OpenCV::opencv
)

option(MAKE_TESTS "Make Tests" ON)

if(${MAKE_TESTS})
    find_package(gtest REQUIRED)

endif()