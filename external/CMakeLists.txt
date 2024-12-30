message(STATUS "Add CPM.cmake")
# download CPM.cmake
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake
        ${CMAKE_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_BINARY_DIR}/cmake/CPM.cmake)

message(STATUS "Fetching external libraries")

message(STATUS "Fetching juce")
CPMAddPackage(
        NAME juce
        URL https://github.com/juce-framework/JUCE/archive/refs/tags/8.0.4.tar.gz
)

message(STATUS "Fetching clap juce extension")
CPMAddPackage(
        NAME clap-juce-extension
        GITHUB_REPOSITORY free-audio/clap-juce-extensions
        GIT_TAG 24e70f7f7cde2842528bb66ff50260b1dc0f4dae
)
