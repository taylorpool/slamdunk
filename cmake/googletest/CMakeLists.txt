include(ExternalProject)

ExternalProject_Add(
    gtest_download
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929
    INSTALL_COMMAND ""
    EXCLUDE_FROM_ALL ON
)