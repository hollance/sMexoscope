cmake_minimum_required(VERSION 3.22)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment target")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(SMExoscope VERSION 1.0.2)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

set(CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    add_compile_definitions(NOMINMAX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

# generate PDB/dSYM files for the Release build
if(MSVC)
    add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Zi>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:REF>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "YES")
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] "dwarf-with-dsym")
    set(CMAKE_XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[variant=Release] "YES")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
endif()

# the external folder will automatically pull JUCE
add_subdirectory(external)

juce_add_plugin(sMexoscope
    PRODUCT_NAME "sMexoscope"
    PLUGIN_NAME "s(M)exoscope"
    COMPANY_NAME "Smartelectronix"
    COMPANY_WEBSITE "https://github.com/hollance/sMexoscope"
    PLUGIN_MANUFACTURER_CODE Manu
    PLUGIN_CODE Pjfj
    BUNDLE_ID "com.smartelectronix.sMexoscope"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    FORMATS AU VST3 LV2 CLAP
)

juce_generate_juce_header(sMexoscope)

file(GLOB_RECURSE PLUGINFILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Source/*)

target_sources(sMexoscope PUBLIC ${PLUGINFILES})

source_group(Sources Source)

target_include_directories(sMexoscope PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

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

target_compile_definitions(sMexoscope PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=1
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
    JUCE_REPORT_APP_USAGE=0
    JUCE_MODAL_LOOPS_PERMITTED=0
    DONT_SET_USING_JUCE_NAMESPACE=1
)

target_link_libraries(sMexoscope
    PRIVATE
        sMexoscopeBinaryData
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

clap_juce_extensions_plugin(TARGET sMexoscope
    CLAP_ID "com.smartelectronix.sMexoscope"
    CLAP_FEATURES CLAP_PLUGIN_FEATURE_AUDIO_EFFECT "oscilloscope"
)

set_target_properties(
    "sMexoscope_CLAP"
    PROPERTIES
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER
    "com.smartelectronix.sMexoscope"
)

if(MSVC)
    target_compile_options(sMexoscope PRIVATE "/W4")
    target_compile_options(sMexoscope PUBLIC "/EHsc")
else()
    target_compile_options(sMexoscope PRIVATE "-Wall")
    target_compile_options(sMexoscope PRIVATE "-Wextra")
    target_compile_options(sMexoscope PRIVATE "-fpermissive")
endif()
