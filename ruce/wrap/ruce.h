#pragma once

#include "juce_conf.h"
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_plugin_client/juce_audio_plugin_client.h>

// VST 3
#include "library_code/patch_juce_audio_plugin_client_VST3.cpp"

// Mac workarounds
#if JUCE_MAC
typedef __CFBundle CFBundleAlias;
#endif

// import some cxx tools
#include "rust/cxx.h"

// import Rust code defined in 
#include "ruce/src/ruce_types.rs.h"

// The main AudioProcessor class that extends JUCE Audio Processor
// It will delegate all the calls to the Rust code instead.
class RuceAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    RuceAudioProcessor() : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                                              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                                              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                           )
    {

    }

    ~RuceAudioProcessor()
    {
        // _implWrapper should be disposed automatically
    }

    // Sets the rust wrapper
    void setImplWrapper(rust::Box<::PluginProcessorImpl> implWrapper)
    {
        // move the impl wrapper and own memory
        _implWrapper = std::make_unique<rust::Box<::PluginProcessorImpl>>(std::move(implWrapper));
    }

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        assert(_implWrapper != nullptr);

        // // grab a mut ref via the boxed impl
        PluginProcessorImpl& w = **(_implWrapper.get());
        // // delegate
        prepare_to_play(w, sampleRate, samplesPerBlock);
    }

    void releaseResources() override
    {
        // When playback stops, you can use this as an opportunity to free up any
        // spare memory, etc.
    }

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override
    {
#if JucePlugin_IsMidiEffect
        juce::ignoreUnused(layouts);
        return true;
#else
        // This is the place where you check if the layout is supported.
        // In this template code we only support mono or stereo.
        // Some plugin hosts, such as certain GarageBand versions, will only
        // load plugins that support stereo bus layouts.
        if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

            // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
        if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
            return false;
#endif

        return true;
#endif
    }

    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override
    {
        juce::ignoreUnused(midiMessages);

        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        // In case we have more outputs than inputs, this code clears any output
        // channels that didn't contain input data, (because these aren't
        // guaranteed to be empty - they may contain garbage).
        // This is here to avoid people getting screaming feedback
        // when they first compile a plugin, but obviously you don't need to keep
        // this code if your algorithm always overwrites all the output channels.
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear(i, 0, buffer.getNumSamples());

        // This is the place where you'd normally do the guts of your plugin's
        // audio processing...
        // Make sure to reset the state if your inner loop is processing
        // the samples and the outer loop is handling the channels.
        // Alternatively, you can process the samples with the channels
        // interleaved by keeping the same state.
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto *channelData = buffer.getWritePointer(channel);
            juce::ignoreUnused(channelData);
            // ..do something to the data...
        }
    }
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override
    {
        // for now
        return nullptr;
    }

    bool hasEditor() const override
    {
        // for now
        return false;
    }

    //==============================================================================
    const juce::String getName() const override
    {
        return JucePlugin_Name;
    }

    bool acceptsMidi() const override
    {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }

    bool producesMidi() const override
    {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    bool isMidiEffect() const override
    {
#if JucePlugin_IsMidiEffect
        return true;
#else
        return false;
#endif
    }

    double getTailLengthSeconds() const override
    {
        return 0.0;
    }

    //==============================================================================
    int getNumPrograms() override
    {
        return 1;
    }

    int getCurrentProgram() override
    {
        return 0;
    }

    void setCurrentProgram(int index) override
    {
        juce::ignoreUnused(index);
    }

    const juce::String getProgramName(int index) override
    {
        juce::ignoreUnused(index);
        return {};
    }

    void changeProgramName(int index, const juce::String &newName) override
    {
        juce::ignoreUnused(index, newName);
    }

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override
    {
        // You should use this method to store your parameters in the memory block.
        // You could do that either as raw data, or use the XML or ValueTree classes
        // as intermediaries to make it easy to save and load complex data.
        juce::ignoreUnused(destData);
    }

    void setStateInformation(const void *data, int sizeInBytes) override
    {
        // You should use this method to restore your parameters from this memory block,
        // whose contents will have been created by the getStateInformation() call.
        juce::ignoreUnused(data, sizeInBytes);
    }

private:
    // manage memory of the wrapper
    std::unique_ptr<rust::Box<::PluginProcessorImpl>> _implWrapper;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RuceAudioProcessor)
};

// This create a new instance of the plugin
// we pass the Rust impl later via the plugin factory
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    RuceAudioProcessor* instance = new RuceAudioProcessor();
    return instance;
}

