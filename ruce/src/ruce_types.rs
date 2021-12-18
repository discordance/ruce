use cxx::{type_id, ExternType};

/// This is the trait to implement for any struct to act as main audio processor.
pub trait PluginProcessor {
    fn new() -> Self where Self: Sized;
    fn prepare_to_play(&mut self, sample_rate: f32, maximum_expected_samples_per_block: usize);
}

// wrapper type to pass through ffi
pub struct PluginProcessorImpl(pub Box<dyn PluginProcessor>);
unsafe impl ExternType for PluginProcessorImpl {
    type Id = type_id!("::PluginProcessorImpl");
    type Kind = cxx::kind::Opaque;
}

// This is a placeholder type, should be overwritten by the crate using this lib
pub struct RealProcessorType {}
impl PluginProcessor for RealProcessorType {
    fn prepare_to_play(&mut self, _sample_rate: f32, _maximum_expected_samples_per_block: usize) {
        println!("{}", "Crate Placeholder");
    }

    fn new() -> Self where Self: Sized {
        return RealProcessorType{};
    }
}

// we need to re-define processor traits methods as global funcs for C++ to call
fn prepare_to_play(imp: &mut PluginProcessorImpl, sample_rate: f32, maximum_expected_samples_per_block: usize) {
    imp.0.prepare_to_play(sample_rate, maximum_expected_samples_per_block);
}

// Creates a placeholder implem
pub fn create_plugin_impl() -> Box<PluginProcessorImpl> {
    let p = PluginProcessorImpl(Box::new(RealProcessorType::new()));
    return Box::new(p);
}

#[cxx::bridge]
pub mod ffi {
    extern "Rust" {
        pub type PluginProcessorImpl;

        fn create_plugin_impl() -> Box<PluginProcessorImpl>;

        // plugin processor functions exposed to C++
        fn prepare_to_play(imp: &mut PluginProcessorImpl, sample_rate: f32, maximum_expected_samples_per_block: usize);
    }
}
