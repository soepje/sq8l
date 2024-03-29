juce_set_vst2_sdk_path(../../third_party/vstsdk2.4)

juce_add_plugin(AudioPluginExample
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        # COMPANY_NAME ...                          # Specify the name of the plugin's author
        IS_SYNTH TRUE                       # Is this a synth or an effect?
        NEEDS_MIDI_INPUT TRUE               # Does the plugin need midi input?
        # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
        # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
        # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE Juce               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE Sq8l                            # A unique four-character plugin id with exactly one upper-case character
        # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
        FORMATS VST Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        PRODUCT_NAME "SQ8L")        # The name of the final executable, which can differ from the target name

set(CMAKE_CXX_STANDARD 17)

target_sources(AudioPluginExample
        PRIVATE
        LcdScreen.cpp
        LcdButton.cpp
        ProgramManager.cpp
        Preset.cpp
        PluginEditor.cpp
        ParameterHelper.cpp
        LookAndFeel.cpp
        PluginProcessor.cpp)

target_compile_definitions(AudioPluginExample
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_include_directories(AudioPluginExample PUBLIC engine)

target_link_libraries(AudioPluginExample
        PRIVATE
        engine
        data

        juce::juce_audio_utils
        juce::juce_gui_basics
        juce::juce_graphics
        juce::juce_audio_processors

        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
