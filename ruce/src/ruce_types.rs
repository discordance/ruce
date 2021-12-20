use cxx::{type_id, ExternType};

/// This is the trait to implement for any struct to act as main audio processor.
pub trait PluginProcessor {
    fn new() -> Self
    where
        Self: Sized;

    /// This function is called by the JUCE AudioProcessor when it is ready to start playing.
    /// This is where you would prepare your plugin to play
    /// For example, you might need to create some buffers
    /// or initialize some variables.  
    fn prepare_to_play(&mut self, sample_rate: f32, maximum_expected_samples_per_block: usize);

    /// This is the main audio callback.
    /// In process_block, you will be passed the block of audio data to process.
    /// JUCE will call this function passing an interleaved (channels) mutable audio slice.
    /// Runs in the audio thread.
    /// 
    /// audio_data: A slice of audio data, channels are interleaved.
    /// num_channels: The number of channels in the audio data.
    /// num_samples: The number of samples for each channels in the audio data.
    fn process_block(
        &mut self,
        audio_data: &mut [f32],
        num_channels: usize,
        num_samples: usize,
    );
}

// wrapper type to pass through ffi
pub struct PluginProcessorImpl(pub Box<dyn PluginProcessor>);
unsafe impl ExternType for PluginProcessorImpl {
    type Id = type_id!("::PluginProcessorImpl");
    type Kind = cxx::kind::Opaque;
}

// we need to re-define processor traits methods as global funcs for C++ to call
fn prepare_to_play(
    imp: &mut PluginProcessorImpl,
    sample_rate: f32,
    maximum_expected_samples_per_block: usize,
) {
    imp.0
        .prepare_to_play(sample_rate, maximum_expected_samples_per_block);
}

fn process_block(
    imp: &mut PluginProcessorImpl,
    audio_data: &mut [f32],
    num_channels: usize,
    num_samples: usize,
) {
    imp.0
        .process_block(audio_data, num_channels, num_samples);
}

#[cxx::bridge]
pub mod ffi {
    extern "Rust" {
        type PluginProcessorImpl;

        // plugin processor functions exposed to C++
        fn prepare_to_play(
            imp: &mut PluginProcessorImpl,
            sample_rate: f32,
            maximum_expected_samples_per_block: usize,
        );
        fn process_block(
            imp: &mut PluginProcessorImpl,
            audio_data: &mut [f32],
            num_channels: usize,
            num_samples: usize,
        );

        // needed to generate Box<PluginProcessorImpl> symbols ?
        fn create_dummy_plugin_impl() -> Box<PluginProcessorImpl>;
    }
}

// This is a placeholder type, should be overwritten by the crate using this lib
pub struct RealProcessorType {}
impl PluginProcessor for RealProcessorType {
    fn new() -> Self
    where
        Self: Sized,
    {
        return RealProcessorType {};
    }

    fn prepare_to_play(&mut self, sample_rate: f32, maximum_expected_samples_per_block: usize) {
        println!(
            "Crate Placeholder {} {}",
            sample_rate, maximum_expected_samples_per_block
        );
    }

    fn process_block(
        &mut self,
        _audio_data: &mut [f32],
        _num_channels: usize,
        _num_samples: usize,
    ) {
    }
}

// Creates a placeholder implem
pub fn create_dummy_plugin_impl() -> Box<PluginProcessorImpl> {
    let p = PluginProcessorImpl(Box::new(RealProcessorType::new()));
    return Box::new(p);
}
