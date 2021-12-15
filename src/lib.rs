// Extract meat from C++ 
#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("ruce/wrap/ruce.h");

        // wrapper type
        type RuceAudioProcessor;

        #[namespace = "juce"]
        type AudioProcessor;

        // this should be found in the global namespace
        fn createPluginFilter() -> *mut AudioProcessor;

        // VST3
        #[namespace = "Steinberg"]
        type IPluginFactory;

        fn GetPluginFactory() -> *mut IPluginFactory;
    }
}


#[no_mangle]
pub fn lola_aime_ca() {

}

#[no_mangle]
#[allow(non_snake_case)]
fn GetPluginFactory() -> *mut ffi::IPluginFactory {
    return ffi::GetPluginFactory();
}


#[cfg(test)]
mod tests {

    #[test]
    fn test() {

    }
}
