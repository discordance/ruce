use ruce_bindings::ffi::{self};

#[macro_export]
macro_rules! create_processor {
    ($a:ty) => {
        #[no_mangle]
        #[allow(non_snake_case)]
        unsafe fn GetPluginFactory() -> *mut ffi::IPluginFactory {
            // @Placeholder
            let pf: *mut ffi::IPluginFactory = ffi::GetPluginFactoryShim(|| {
                let p = Box::new(PluginProcessorImpl(Box::new(<$a>::new())));
                Box::<PluginProcessorImpl>::into_raw(p)
            });
            return pf;
        }
    };
}

// entry point on mac
#[no_mangle]
#[allow(non_snake_case)]
#[cfg(target_os = "macos")]
fn bundleEntry(r: *mut ffi::CFBundleAlias) -> bool {
    return unsafe { ffi::bundleEntry(r) };
}

// exit point on mac
#[no_mangle]
#[allow(non_snake_case)]
#[cfg(target_os = "macos")]
fn bundleExit() -> bool {
    return ffi::bundleExit();
}
