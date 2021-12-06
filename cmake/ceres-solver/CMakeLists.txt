include(FetchContent)
project(Ceres)

FetchContent_Declare(
    ceres-solver
    GIT_REPOSITORY https://ceres-solver.googlesource.com/ceres-solver
    GIT_TAG 0ffb6db49cec52566f53ab66be7a19970ddc562b
)