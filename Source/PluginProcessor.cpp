/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AetherAudioProcessor::AetherAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
    apvts(*this, nullptr, "Parameters", createParameters())
{
    apvts.state = juce::ValueTree("Parameters");
}

AetherAudioProcessor::~AetherAudioProcessor()
{
}

//==============================================================================
const juce::String AetherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AetherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AetherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AetherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AetherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AetherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AetherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AetherAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AetherAudioProcessor::getProgramName (int index)
{
    return {};
}

void AetherAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AetherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AetherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AetherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AetherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool AetherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AetherAudioProcessor::createEditor()
{
    return new AetherAudioProcessorEditor (*this);
}

//==============================================================================
void AetherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
	std::unique_ptr <juce::XmlElement> params(apvts.state.createXml());

	copyXmlToBinary(*params, destData);
    
}

void AetherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr <juce::XmlElement> params(getXmlFromBinary(data, sizeInBytes));

    if (params != nullptr) {
        if (params->hasTagName(apvts.state.getType())) {
            apvts.state = juce::ValueTree::fromXml(*params);
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout AetherAudioProcessor::createParameters()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("leftChannelPan", "Left Channel Pan", -1.0f, 1.0f, -1.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("leftChannelGain", "Left Channel Gain", -20.0f, 20.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("rightChannelPan", "Right Channel Pan", -1.0f, 1.0f, 1.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("rightChannelGain", "Right Channel Gain", -20.0f, 20.0f, 0.0f));

	return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AetherAudioProcessor();
}
