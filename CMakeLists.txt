cmake_minimum_required(VERSION 3.22)

project(SMExoscope VERSION 1.0.1)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (MSVC)
    add_compile_definitions(NOMINMAX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif ()

# the external folder will automatically pull JUCE
add_subdirectory(external)

juce_add_binary_data(sMexoscopeBinaryData SOURCES
        Images/blue_knob1_4.png
        Images/body.png
        Images/free_etc.png
        Images/heads.png
        Images/lefr_right.png
        Images/off_on.png
        Images/readout.png
        Images/slider_new.png
)

add_subdirectory(Source)

