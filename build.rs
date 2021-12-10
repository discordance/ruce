use std::env;

fn main() {
    // target OS
    let target_os = env::var("CARGO_CFG_TARGET_OS");

    // include paths
    let wrap_path = std::path::PathBuf::from("wrap");
    let juce_path = std::path::PathBuf::from(".juce/modules");

    // the builder
    let mut b = autocxx_build::Builder::new("src/lib.rs", &[&wrap_path, &juce_path]).expect_build();
    b.cpp(true);

    // flags
    b.flag_if_supported("-std=c++17");

    // modules compilation
    match target_os.as_ref().map(|x| &**x) {
        Ok("macos") | Ok("ios") => {
            b.file("wrap/library_code/include_juce_core.mm");
        }
        _ => {
            b.file("wrap/library_code/include_juce_core.cpp");
        }
    }

    // compile    
    b.compile("autocxx-ruce");

    // platform specific
    match target_os.as_ref().map(|x| &**x) {
        Ok("macos") | Ok("ios") => {
            println!("cargo:rustc-link-lib=framework=Foundation");
            println!("cargo:rustc-link-lib=framework=Cocoa");
        }
        _ => {}
    }

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=wrap/defs.h");
    println!("cargo:rerun-if-changed=wrap/juce_conf.h");

    // lib files
    println!("cargo:rerun-if-changed=wrap/library_code/include_juce_core.cpp");
    println!("cargo:rerun-if-changed=wrap/library_code/include_juce_core.mm");
}
