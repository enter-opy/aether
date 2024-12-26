/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AetherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AetherAudioProcessorEditor (AetherAudioProcessor&);
    ~AetherAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AetherAudioProcessor& audioProcessor;

    juce::Image companyLogo = juce::ImageCache::getFromMemory(BinaryData::company_png, BinaryData::company_pngSize);

    juce::Rectangle<float> xyPad;

    juce::Point<float> thumbPos[2];
    float thumbRadius = 10.0f;
    int draggedThumb = false;

    juce::Slider panSlider[2];
    juce::Slider gainSlider[2];

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panAttachment[2];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment[2];

    juce::Typeface::Ptr customTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::DotGothic16Regular_ttf, BinaryData::DotGothic16Regular_ttfSize);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AetherAudioProcessorEditor)
};
