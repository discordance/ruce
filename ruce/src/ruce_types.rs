use cxx::{type_id, ExternType};

/// This is the trait to implement for any struct to act as main audio processor.
pub trait PluginProcessor {
    fn new() -> Self where Self: Sized;
    fn prepare_to_play(&mut self, sample_rate: f32, maximum_expected_samples_per_block: usize);
    fn process_block(&mut self, flat_audio_data:&[f32]);
}

// wrapper type to pass through ffi
pub struct PluginProcessorImpl(pub Box<dyn PluginProcessor>);
unsafe impl ExternType for PluginProcessorImpl {
    type Id = type_id!("::PluginProcessorImpl");
    type Kind = cxx::kind::Opaque;
}

// we need to re-define processor traits methods as global funcs for C++ to call
fn prepare_to_play(imp: &mut PluginProcessorImpl, sample_rate: f32, maximum_expected_samples_per_block: usize) {
    imp.0.prepare_to_play(sample_rate, maximum_expected_samples_per_block);
}

fn process_block(imp: &mut PluginProcessorImpl, flat_audio_data:&[f32]) {
    imp.0.process_block(flat_audio_data);
}


#[cxx::bridge]
pub mod ffi {
    extern "Rust" {
        type PluginProcessorImpl;

        fn create_dummy_plugin_impl() -> Box<PluginProcessorImpl>;

        // plugin processor functions exposed to C++
        fn prepare_to_play(imp: &mut PluginProcessorImpl, sample_rate: f32, maximum_expected_samples_per_block: usize);
        fn process_block(imp: &mut PluginProcessorImpl, flat_audio_data:&[f32]);
    }
}

// This is a placeholder type, should be overwritten by the crate using this lib
pub struct RealProcessorType {}
impl PluginProcessor for RealProcessorType {
    fn new() -> Self where Self: Sized {
        return RealProcessorType{};
    }

    fn prepare_to_play(&mut self, sample_rate: f32, maximum_expected_samples_per_block: usize) {
        println!("Crate Placeholder {} {}", sample_rate, maximum_expected_samples_per_block);
    }

    fn process_block(&mut self, flat_audio_data:&[f32]) {
        
    }
}

// Creates a placeholder implem
pub fn create_dummy_plugin_impl() -> Box<PluginProcessorImpl> {
    let p = PluginProcessorImpl(Box::new(RealProcessorType::new()));
    return Box::new(p);
}