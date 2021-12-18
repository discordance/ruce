pub mod ruce_types;

// Extract meat from C++ 
#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("ruce/wrap/ruce.h");
        include!("ruce/wrap/vst3_shim.h");

        // wrapper type
        type RuceAudioProcessor;

        #[namespace = "juce"]
        type AudioProcessor;

        // redeclar for local bridge functions
        type PluginProcessorImpl = crate::ruce_types::PluginProcessorImpl;

        // VST3
        #[namespace = "Steinberg"]
        type IPluginFactory;

        // Mac
        type CFBundleAlias;
        
        #[namespace = "ruce"]
        unsafe fn GetPluginFactoryShim(_impl: *mut PluginProcessorImpl) -> *mut IPluginFactory;

        unsafe fn bundleEntry(r: *mut CFBundleAlias) -> bool;
        fn bundleExit() -> bool;
    }
}