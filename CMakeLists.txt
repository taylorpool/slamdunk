cmake_minimum_required(VERSION 3.15)
project(SLAMDunk)

# find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
# find_package(glog REQUIRED)
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 17)

add_library(
    SLAMDunk
    # Put source files here
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



option(MAKE_TESTS "Make Tests" OFF)

# if(${MAKE_TESTS})
#     find_package(gtest REQUIRED)

# endif()