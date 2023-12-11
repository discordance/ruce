pub mod ruce_types;

// Extract meat from C++ 
#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("ruce_bindings/wrap/ruce.h");
        include!("ruce_bindings/wrap/vst3_shim.h");

        // wrapper type
        type RuceAudioProcessor;

        #[namespace = "juce"]
        type AudioProcessor;

        #[namespace = "ruce"]
        type FloatBuffer;

        // redeclar for local bridge functions
        type PluginProcessorImpl = crate::ruce_types::PluginProcessorImpl;

        // VST3
        #[namespace = "Steinberg"]
        type IPluginFactory;

        // Mac
        type CFBundleAlias;
        
        #[namespace = "ruce"]
        unsafe fn GetPluginFactoryShim(_impl: fn() -> *mut PluginProcessorImpl) -> *mut IPluginFactory;
        
        unsafe fn bundleEntry(r: *mut CFBundleAlias) -> bool;
        fn bundleExit() -> bool;
    }
}