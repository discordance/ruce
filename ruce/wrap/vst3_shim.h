namespace ruce
{
    using BoxedAudioWrapper = rust::Box<PluginProcessorImpl>;

    using CreateFunction = std::function<FUnknown*(Vst::IHostApplication *)>;

    // factory SHIM
    // We need a more flexible factory. Needs to support proper std::function instead of CreateFunction
    struct RucePluginFactory;
    static RucePluginFactory *globalFactory = nullptr;
    struct RucePluginFactory : public IPluginFactory3
    {
        RucePluginFactory()
            : factoryInfo(JucePlugin_Manufacturer, JucePlugin_ManufacturerWebsite,
                          JucePlugin_ManufacturerEmail, Vst::kDefaultFactoryFlags)
        {
        }

        virtual ~RucePluginFactory()
        {
            if (globalFactory == this)
                globalFactory = nullptr;
        }

        //==============================================================================
        bool registerClass(const PClassInfo2 &info, CreateFunction createFunction)
        {
            if (createFunction == nullptr)
            {
                jassertfalse;
                return false;
            }

            auto entry = std::make_unique<ClassEntry>(info, createFunction);
            entry->infoW.fromAscii(info);

            classes.push_back(std::move(entry));

            return true;
        }

        //==============================================================================
        JUCE_DECLARE_VST3_COM_REF_METHODS

        tresult PLUGIN_API queryInterface(const TUID targetIID, void **obj) override
        {
            const auto result = testForMultiple(*this,
                                                targetIID,
                                                UniqueBase<IPluginFactory3>{},
                                                UniqueBase<IPluginFactory2>{},
                                                UniqueBase<IPluginFactory>{},
                                                UniqueBase<FUnknown>{});

            if (result.isOk())
                return result.extract(obj);

            jassertfalse; // Something new?
            *obj = nullptr;
            return kNotImplemented;
        }

        //==============================================================================
        Steinberg::int32 PLUGIN_API countClasses() override
        {
            return (Steinberg::int32)classes.size();
        }

        tresult PLUGIN_API getFactoryInfo(PFactoryInfo *info) override
        {
            if (info == nullptr)
                return kInvalidArgument;

            memcpy(info, &factoryInfo, sizeof(PFactoryInfo));
            return kResultOk;
        }

        tresult PLUGIN_API getClassInfo(Steinberg::int32 index, PClassInfo *info) override
        {
            return getPClassInfo<PClassInfo>(index, info);
        }

        tresult PLUGIN_API getClassInfo2(Steinberg::int32 index, PClassInfo2 *info) override
        {
            return getPClassInfo<PClassInfo2>(index, info);
        }

        tresult PLUGIN_API getClassInfoUnicode(Steinberg::int32 index, PClassInfoW *info) override
        {
            if (info != nullptr)
            {
                if (auto &entry = classes[(size_t)index])
                {
                    memcpy(info, &entry->infoW, sizeof(PClassInfoW));
                    return kResultOk;
                }
            }

            return kInvalidArgument;
        }

        tresult PLUGIN_API createInstance(FIDString cid, FIDString sourceIid, void **obj) override
        {
            ScopedJuceInitialiser_GUI libraryInitialiser;

#if JUCE_LINUX || JUCE_BSD
            SharedResourcePointer<MessageThread> messageThread;
#endif

            *obj = nullptr;

            TUID tuid;
            memcpy(tuid, sourceIid, sizeof(TUID));

#if VST_VERSION >= 0x030608
            auto sourceFuid = FUID::fromTUID(tuid);
#else
            FUID sourceFuid;
            sourceFuid = tuid;
#endif

            if (cid == nullptr || sourceIid == nullptr || !sourceFuid.isValid())
            {
                jassertfalse; // The host you're running in has severe implementation issues!
                return kInvalidArgument;
            }

            TUID iidToQuery;
            sourceFuid.toTUID(iidToQuery);

            for (auto &entry : classes)
            {
                if (doUIDsMatch(entry->infoW.cid, cid))
                {
                    if (auto *instance = entry->createFunction(host))
                    {
                        const FReleaser releaser(instance);

                        if (instance->queryInterface(iidToQuery, obj) == kResultOk)
                            return kResultOk;
                    }

                    break;
                }
            }

            return kNoInterface;
        }

        tresult PLUGIN_API setHostContext(FUnknown *context) override
        {
            host.loadFrom(context);

            if (host != nullptr)
            {
                Vst::String128 name;
                host->getName(name);

                return kResultTrue;
            }

            return kNotImplemented;
        }

    private:
        //==============================================================================
        std::atomic<int> refCount{1};
        const PFactoryInfo factoryInfo;
        VSTComSmartPtr<Vst::IHostApplication> host;

        //==============================================================================
        struct ClassEntry
        {
            ClassEntry() noexcept {}

            ClassEntry(const PClassInfo2 &info, CreateFunction fn) noexcept
                : info2(info), createFunction(fn) {}

            PClassInfo2 info2;
            PClassInfoW infoW;
            CreateFunction createFunction = {};
            bool isUnicode = false;

        private:
            JUCE_DECLARE_NON_COPYABLE(ClassEntry)
        };

        std::vector<std::unique_ptr<ClassEntry>> classes;

        //==============================================================================
        template <class PClassInfoType>
        tresult PLUGIN_API getPClassInfo(Steinberg::int32 index, PClassInfoType *info)
        {
            if (info != nullptr)
            {
                zerostruct(*info);

                if (auto &entry = classes[(size_t)index])
                {
                    if (entry->isUnicode)
                        return kResultFalse;

                    memcpy(info, (PClassInfoType *)&entry->info2, sizeof(PClassInfoType));
                    return kResultOk;
                }
            }

            jassertfalse;
            return kInvalidArgument;
        }

        //==============================================================================
        // no leak detector here to prevent it firing on shutdown when running in hosts that
        // don't release the factory object correctly...
        JUCE_DECLARE_NON_COPYABLE(RucePluginFactory)
    };

    // needs custom components
    class RuceVST3Component : public JuceVST3Component
    {
    public:
        RuceVST3Component(Vst::IHostApplication *h, ::rust::Fn<::PluginProcessorImpl*()> _implCreate) : JuceVST3Component(h)
        {
            RuceAudioProcessor &aP = (RuceAudioProcessor &)getPluginInstance();
            // use the rust closure tu create a new rust wrapped processor
            PluginProcessorImpl* new_impl = _implCreate();
            auto boxedImpl = rust::Box<PluginProcessorImpl>::from_raw(new_impl);
            aP.setImplWrapper(std::move(boxedImpl));
        }
    };

    // replacing juce's GetPluginFactory
    IPluginFactory *PLUGIN_API GetPluginFactoryShim(::rust::Fn<::PluginProcessorImpl*()> _implCreate)
    {
        PluginHostType::jucePlugInClientCurrentWrapperType = AudioProcessor::wrapperType_VST3;

        // pass the wrapper creation closure
        auto createPlugin = [_implCreate](Vst::IHostApplication *host) -> FUnknown *
        {
            return static_cast<Vst::IAudioProcessor *>(new RuceVST3Component(host, _implCreate));
        };

#if (JUCE_MSVC || (JUCE_WINDOWS && JUCE_CLANG)) && JUCE_32BIT
// Cunning trick to force this function to be exported. Life's too short to
// faff around creating .def files for this kind of thing.
// Unnecessary for 64-bit builds because those don't use decorated function names.
#pragma comment(linker, "/EXPORT:GetPluginFactory=_GetPluginFactory@0")
#endif
        if (globalFactory == nullptr)
        {
            globalFactory = new RucePluginFactory();

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
            globalFactory->registerClass(
                componentClass, createPlugin);

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