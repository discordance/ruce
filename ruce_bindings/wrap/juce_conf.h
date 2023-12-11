// All this should be defined dynamically
// @TODO check how to override that dynamically

// META
#ifndef  JucePlugin_Name
 #define JucePlugin_Name                   "RUCE"
#endif
#ifndef  JucePlugin_Desc
 #define JucePlugin_Desc                   "RUCE"
#endif
#ifndef  JucePlugin_Manufacturer
 #define JucePlugin_Manufacturer           "RUCE Inc"
#endif
#ifndef  JucePlugin_ManufacturerWebsite
 #define JucePlugin_ManufacturerWebsite    ""
#endif
#ifndef  JucePlugin_ManufacturerEmail
 #define JucePlugin_ManufacturerEmail      "ruce@ruce.com"
#endif
#ifndef  JucePlugin_ManufacturerCode
 #define JucePlugin_ManufacturerCode       0x46415731
#endif
#ifndef  JucePlugin_PluginCode
 #define JucePlugin_PluginCode             0x5362586c
#endif
#ifndef  JucePlugin_IsSynth
 #define JucePlugin_IsSynth                0
#endif
#ifndef  JucePlugin_WantsMidiInput
 #define JucePlugin_WantsMidiInput         1
#endif
#ifndef  JucePlugin_ProducesMidiOutput
 #define JucePlugin_ProducesMidiOutput     1
#endif
#ifndef  JucePlugin_IsMidiEffect
 #define JucePlugin_IsMidiEffect           0
#endif
#ifndef  JucePlugin_EditorRequiresKeyboardFocus
 #define JucePlugin_EditorRequiresKeyboardFocus  0
#endif
#ifndef  JucePlugin_Version
 #define JucePlugin_Version                1
#endif
#ifndef  JucePlugin_VersionCode
 #define JucePlugin_VersionCode            0x10000
#endif
#ifndef  JucePlugin_VersionString
 #define JucePlugin_VersionString          "1.0.0"
#endif
#ifndef  JucePlugin_VSTUniqueID
 #define JucePlugin_VSTUniqueID            JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_VSTCategory
 #define JucePlugin_VSTCategory            kPlugCategEffect
#endif
#ifndef  JucePlugin_Vst3Category
 #define JucePlugin_Vst3Category           "Instrument|Synth"
#endif
//==============================================================================


//==============================================================================
// Replacement for JucePluginDefine

#ifndef JucePlugin_Build_VST3
#define JucePlugin_Build_VST3 1
#endif

//==============================================================================
// Replacement for AppConfig.h

#ifndef JUCE_DISPLAY_SPLASH_SCREEN
#define JUCE_DISPLAY_SPLASH_SCREEN 0
#endif

#define JUCE_USE_DARK_SPLASH_SCREEN 0
#define JUCE_PROJUCER_VERSION 0x0

#define JUCE_MODULE_AVAILABLE_juce_audio_basics 1
#define JUCE_MODULE_AVAILABLE_juce_audio_devices 0
#define JUCE_MODULE_AVAILABLE_juce_audio_formats 0
#define JUCE_MODULE_AVAILABLE_juce_audio_plugin_client 0
#define JUCE_MODULE_AVAILABLE_juce_audio_processors 1
#define JUCE_MODULE_AVAILABLE_juce_audio_utils 0
#define JUCE_MODULE_AVAILABLE_juce_core 1
#define JUCE_MODULE_AVAILABLE_juce_cryptography 0
#define JUCE_MODULE_AVAILABLE_juce_data_structures 1
#define JUCE_MODULE_AVAILABLE_juce_dsp 0
#define JUCE_MODULE_AVAILABLE_juce_events 1
#define JUCE_MODULE_AVAILABLE_juce_graphics 1
#define JUCE_MODULE_AVAILABLE_juce_gui_basics 1
#define JUCE_MODULE_AVAILABLE_juce_gui_extra 0
#define JUCE_MODULE_AVAILABLE_juce_opengl 0
#define JUCE_MODULE_AVAILABLE_juce_product_unlocking 0

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1

#define JUCE_STANDALONE_APPLICATION 1

//==============================================================================
// juce_core flags:

#ifndef JUCE_FORCE_DEBUG
#define JUCE_FORCE_DEBUG 0
#endif

#ifndef JUCE_LOG_ASSERTIONS
#define JUCE_LOG_ASSERTIONS 0
#endif

#ifndef JUCE_CHECK_MEMORY_LEAKS
#define JUCE_CHECK_MEMORY_LEAKS 1
#endif

#ifndef JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
#define JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES 0
#endif

#ifndef JUCE_INCLUDE_ZLIB_CODE
#define JUCE_INCLUDE_ZLIB_CODE 1
#endif

#ifndef JUCE_USE_CURL
#define JUCE_USE_CURL 0
#endif

#ifndef JUCE_LOAD_CURL_SYMBOLS_LAZILY
#define JUCE_LOAD_CURL_SYMBOLS_LAZILY 0
#endif

#ifndef JUCE_CATCH_UNHANDLED_EXCEPTIONS
#define JUCE_CATCH_UNHANDLED_EXCEPTIONS 0
#endif

#ifndef JUCE_ALLOW_STATIC_NULL_VARIABLES
#define JUCE_ALLOW_STATIC_NULL_VARIABLES 0
#endif

#ifndef JUCE_STRICT_REFCOUNTEDPOINTER
#define JUCE_STRICT_REFCOUNTEDPOINTER 1
#endif

#ifndef JUCE_ENABLE_ALLOCATION_HOOKS
#define JUCE_ENABLE_ALLOCATION_HOOKS 0
#endif

//==============================================================================
// juce_audio_plugin_client flags:

#ifndef JUCE_VST3_CAN_REPLACE_VST2
#define JUCE_VST3_CAN_REPLACE_VST2 0
#endif

#ifndef JUCE_FORCE_USE_LEGACY_PARAM_IDS
#define JUCE_FORCE_USE_LEGACY_PARAM_IDS 0
#endif

#ifndef JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE
#define JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE 0
#endif

#ifndef JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS
#define JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS 0
#endif

#ifndef JUCE_AU_WRAPPERS_SAVE_PROGRAM_STATES
#define JUCE_AU_WRAPPERS_SAVE_PROGRAM_STATES 0
#endif

#ifndef JUCE_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE
#define JUCE_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE 0
#endif

//==============================================================================
// juce_audio_processors flags:

#ifndef JUCE_PLUGINHOST_VST
#define JUCE_PLUGINHOST_VST 0
#endif

#ifndef JUCE_PLUGINHOST_VST3
#define JUCE_PLUGINHOST_VST3 1
#endif

#ifndef JUCE_PLUGINHOST_AU
#define JUCE_PLUGINHOST_AU 0
#endif

#ifndef JUCE_PLUGINHOST_LADSPA
#define JUCE_PLUGINHOST_LADSPA 0
#endif

#ifndef JUCE_CUSTOM_VST3_SDK
#define JUCE_CUSTOM_VST3_SDK 0
#endif

// ------------ other conf
#define DEBUG 1
#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1