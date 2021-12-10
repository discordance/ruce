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
// #define JUCE_PROJUCER_VERSION 0x60102

#define JUCE_MODULE_AVAILABLE_juce_audio_basics 1
#define JUCE_MODULE_AVAILABLE_juce_audio_devices 0
#define JUCE_MODULE_AVAILABLE_juce_audio_formats 0
#define JUCE_MODULE_AVAILABLE_juce_audio_plugin_client 0
#define JUCE_MODULE_AVAILABLE_juce_audio_processors 0
#define JUCE_MODULE_AVAILABLE_juce_audio_utils 0
#define JUCE_MODULE_AVAILABLE_juce_core 1
#define JUCE_MODULE_AVAILABLE_juce_cryptography 0
#define JUCE_MODULE_AVAILABLE_juce_data_structures 0
#define JUCE_MODULE_AVAILABLE_juce_dsp 0
#define JUCE_MODULE_AVAILABLE_juce_events 0
#define JUCE_MODULE_AVAILABLE_juce_graphics 0
#define JUCE_MODULE_AVAILABLE_juce_gui_basics 0
#define JUCE_MODULE_AVAILABLE_juce_gui_extra 0
#define JUCE_MODULE_AVAILABLE_juce_opengl 0
#define JUCE_MODULE_AVAILABLE_juce_product_unlocking 0

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1

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
#define JUCE_USE_CURL 1
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

// ------------ other conf
#define DEBUG 1
#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1