use std::env;

fn main() {
    // target OS
    let target_os = env::var("CARGO_CFG_TARGET_OS");

    // the builder
    let mut b = cxx_build::bridge("src/lib.rs");

    // include paths
    b.include("wrap");
    b.include(".juce/modules");
    b.include(".juce/modules/juce_audio_processors/format_types/VST3_SDK");
    b.cpp(true);

    // flags
    b.flag_if_supported("-std=c++17");

    // modules compilation
    match target_os.as_ref().map(|x| &**x) {
        Ok("macos") | Ok("ios") => {
            b.file("wrap/library_code/juce_core.mm");
            b.file("wrap/library_code/juce_audio_basics.mm");
            b.file("wrap/library_code/juce_events.mm");
            b.file("wrap/library_code/juce_data_structures.mm");
            b.file("wrap/library_code/juce_graphics.mm");
            b.file("wrap/library_code/juce_gui_basics.mm");
            b.file("wrap/library_code/juce_audio_processors.mm");
            b.file("wrap/library_code/juce_audio_plugin_client.mm");

            b.file("wrap/library_code/patch_juce_VST3.mm");
        }
        _ => {
            b.file("wrap/library_code/juce_core.cpp");
            b.file("wrap/library_code/juce_audio_basics.cpp");
            b.file("wrap/library_code/juce_events.cpp");
            b.file("wrap/library_code/juce_data_structures.cpp");
            b.file("wrap/library_code/juce_graphics.cpp");
            b.file("wrap/library_code/juce_gui_basics.cpp");
            b.file("wrap/library_code/juce_audio_processors.cpp");
            b.file("wrap/library_code/juce_audio_plugin_client.cpp");
        }
    }

    // compile    
    b.compile("ruce");

    // platform specific linkers
    match target_os.as_ref().map(|x| &**x) {
        Ok("macos") | Ok("ios") => {
            println!("cargo:rustc-link-lib=framework=Foundation");
            println!("cargo:rustc-link-lib=framework=Cocoa");
            println!("cargo:rustc-link-lib=framework=QuartzCore");
            println!("cargo:rustc-link-lib=framework=IOKit");
            // @TODO check if available ? might be useless
            println!("cargo:rustc-link-lib=framework=Accelerate");
        }
        _ => {}
    }

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=wrap/ruce.h");
    println!("cargo:rerun-if-changed=wrap/juce_conf.h");

    // lib files
    println!("cargo:rerun-if-changed=wrap/library_code/juce_core.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_core.mm");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_audio_basics.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_audio_basics.mm");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_events.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_events.mm");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_data_structures.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_data_structures.mm");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_graphics.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_graphics.mm");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_gui_basics.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_gui_basics.mm");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_audio_processors.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_audio_processors.mm");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_audio_plugin_client.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/juce_audio_plugin_client.mm");
}
