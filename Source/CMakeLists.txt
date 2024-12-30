file(GLOB_RECURSE PLUGINFILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*)

juce_add_plugin(sMexoscope
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "Smartelectronix"                          # Specify the name of the plugin's author
        BUNDLE_ID "com.smartelectronix.sMexoscope"
        IS_SYNTH FALSE                       # Is this a synth or an effect?
        NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
        NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
        IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE    # Does the editor need keyboard focus?
        # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE Manu               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE Pjfj                            # A unique four-character plugin id with exactly one upper-case character
        # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
        FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        PRODUCT_NAME "sMexoscope")        # The name of the final executable, which can differ from the target name


juce_generate_juce_header(sMexoscope)

target_sources(sMexoscope PUBLIC ${PLUGINFILES})

target_compile_definitions(sMexoscope
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_include_directories(sMexoscope PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sMexoscope
        PUBLIC
        sMexoscopeBinaryData
        juce::juce_audio_utils
        juce::juce_dsp

        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)


clap_juce_extensions_plugin(TARGET sMexoscope
        CLAP_ID "com.smartelectronix.sMexoscope"
        CLAP_FEATURES CLAP_PLUGIN_FEATURE_AUDIO_EFFECT "oscilloscope"
)

if (MSVC)
    target_compile_options(sMexoscope PRIVATE "/W4")
    target_compile_options(sMexoscope PUBLIC "/EHsc")
else ()
    target_compile_options(sMexoscope PRIVATE "-Wall")
    target_compile_options(sMexoscope PRIVATE "-Wextra")
    target_compile_options(sMexoscope PRIVATE "-fpermissive")
endif ()
