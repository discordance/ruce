namespace ruce
{
    IPluginFactory *PLUGIN_API GetPluginFactoryShim(PluginProcessorImpl *_impl)
    {
        // get ownership of the wrapper, unsafe way
        rust::Box<PluginProcessorImpl> _implOwned = rust::Box<PluginProcessorImpl>::from_raw(_impl);

        PluginHostType::jucePlugInClientCurrentWrapperType = AudioProcessor::wrapperType_VST3;

#if (JUCE_MSVC || (JUCE_WINDOWS && JUCE_CLANG)) && JUCE_32BIT
// Cunning trick to force this function to be exported. Life's too short to
// faff around creating .def files for this kind of thing.
// Unnecessary for 64-bit builds because those don't use decorated function names.
#pragma comment(linker, "/EXPORT:GetPluginFactory=_GetPluginFactory@0")
#endif

        // juce::CreateFunction f = ;

        if (globalFactory == nullptr)
        {
            globalFactory = new JucePluginFactory();

            static const PClassInfo2 componentClass(JuceVST3Component::iid,
                                                    PClassInfo::kManyInstances,
                                                    kVstAudioEffectClass,
                                                    JucePlugin_Name,
                                                    JucePlugin_Vst3ComponentFlags,
                                                    JucePlugin_Vst3Category,
                                                    JucePlugin_Manufacturer,
                                                    JucePlugin_VersionString,
                                                    kVstVersionString);
            // use a closure
            globalFactory->registerClass(componentClass, [] (Vst::IHostApplication *host) -> FUnknown*
                                         { 
                                            RuceAudioProcessor n = new RuceAudioProcessor(std::move(_implOwned));
                                            return static_cast<Vst::IAudioProcessor *>(new JuceVST3Component(host)); 
                                        });

            static const PClassInfo2 controllerClass(JuceVST3EditController::iid,
                                                     PClassInfo::kManyInstances,
                                                     kVstComponentControllerClass,
                                                     JucePlugin_Name,
                                                     JucePlugin_Vst3ComponentFlags,
                                                     JucePlugin_Vst3Category,
                                                     JucePlugin_Manufacturer,
                                                     JucePlugin_VersionString,
                                                     kVstVersionString);

            globalFactory->registerClass(controllerClass, createControllerInstance);
        }
        else
        {
            globalFactory->addRef();
        }

        return dynamic_cast<IPluginFactory *>(globalFactory);
    }
}