if(NOT ${BUILT_OPEN_CV})
include(ExternalProject)

ExternalProject_Add(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv
    GIT_TAG 4.5.4
    INSTALL_COMMAND ""
)
set(BUILT_OPEN_CV ON)
endif()