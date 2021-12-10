use autocxx::include_cpp;

include_cpp! {
    #include "defs.h"
    
    safety!(unsafe)
    generate!("ruce::Dummy")
    generate!("ruce::testing_juce")
}

#[cfg(test)]
mod tests {

    // use crate::ffi::juce::ADSR;
    // use crate::ffi::ruce::create_new_AudioBufferFloat;

    #[test]
    fn test_1() {
        // let adsr = ADSR::make_unique();
        // let ns = adsr.getParameters();

        // let buffer = create_new_AudioBufferFloat();
    }
}
