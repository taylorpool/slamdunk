cmake_minimum_required(VERSION 3.15)
project(SLAMDunk)

# find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
# find_package(glog REQUIRED)
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

add_library(SLAMDunk
# Put source files here
src/load_data.cpp
src/slamdunk.cpp
)

target_include_directories(
    SLAMDunk
    PUBLIC
    "include"
    "${OpenCV_INCLUDE_DIRS}"
)

target_link_libraries(
    SLAMDunk
    PUBLIC
    Ceres::ceres
    "${OpenCV_LIBS}"
)

option(MAKE_TESTS "Make Tests" OFF)

# if(${MAKE_TESTS})
#     find_package(gtest REQUIRED)

# endif()