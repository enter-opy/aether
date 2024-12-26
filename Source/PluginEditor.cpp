/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AetherAudioProcessorEditor::AetherAudioProcessorEditor (AetherAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (420, 500);


    // Sliders
    leftChannelPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "leftChannelPan", leftChannelPanSlider);
    leftChannelGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "leftChannelGain", leftChannelGainSlider);
    rightChannelPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "rightChannelPan", rightChannelPanSlider);
    rightChannelGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "rightChannelGain", rightChannelGainSlider);

    leftChannelPanSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    leftChannelPanSlider.setRange(-1.0f, 1.0f, 0.01f);
    leftChannelPanSlider.setValue(audioProcessor.apvts.getRawParameterValue("leftChannelPan")->load());
    leftChannelPanSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&leftChannelPanSlider);
    leftChannelPanSlider.setEnabled(false);
    leftChannelPanSlider.setVisible(false);

    leftChannelGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    leftChannelGainSlider.setRange(0.0f, 1.0f, 0.01f);
    leftChannelGainSlider.setValue(audioProcessor.apvts.getRawParameterValue("leftChannelGain")->load());
    leftChannelGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&leftChannelGainSlider);
    leftChannelGainSlider.setEnabled(false);
    leftChannelGainSlider.setVisible(false);

    rightChannelPanSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    rightChannelPanSlider.setRange(-1.0f, 1.0f, 0.01f);
    rightChannelPanSlider.setValue(audioProcessor.apvts.getRawParameterValue("rightChannelPan")->load());
    rightChannelPanSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&rightChannelPanSlider);
    rightChannelPanSlider.setEnabled(false);
    rightChannelPanSlider.setVisible(false);

    rightChannelGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    rightChannelGainSlider.setRange(0.0f, 1.0f, 0.01f);
    rightChannelGainSlider.setValue(audioProcessor.apvts.getRawParameterValue("rightChannelGain")->load());
    rightChannelGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&rightChannelGainSlider);
    rightChannelGainSlider.setEnabled(false);
    rightChannelGainSlider.setVisible(false);
}

AetherAudioProcessorEditor::~AetherAudioProcessorEditor()
{
}

//==============================================================================
void AetherAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour::fromRGB(0x1D, 0x0E, 0x0E));
    g.fillAll();

    // Draw logo on top left
    g.drawImage(companyLogo, 15, 25, 49.5, 45, 0, 0, companyLogo.getWidth(), companyLogo.getHeight());

    g.setColour(juce::Colour::fromRGB(0xDE, 0x1C, 0x7C));
    g.setFont(juce::Font(customTypeface)); 
    g.setFont(36);
    g.drawFittedText("Aether", getWidth() / 2.0f - 36, 40 - 18, 72, 36, juce::Justification::centred, 1);

    // Draw XY Pad
    for (int i = 1; i < 8; i++) {
        if (i == 4) {
            g.setColour(juce::Colour::fromRGBA(0xDE, 0x1C, 0x7C, 0x88));
		}
		else {
            g.setColour(juce::Colour::fromRGBA(0xDE, 0x1C, 0x7C, 0x2A));
		}

        g.drawLine(xyPad.getX(), xyPad.getY() + i * (xyPad.getHeight() / 8), xyPad.getRight(), xyPad.getY() + i * (xyPad.getHeight() / 8), 2);
        g.drawLine(xyPad.getX() + i * (xyPad.getWidth() / 8), xyPad.getY(), xyPad.getX() + i * (xyPad.getWidth() / 8), xyPad.getBottom(), 2);

        g.setColour(juce::Colours::white);
        g.setFont(12);
        g.drawFittedText(juce::String((i - 4) / 4.0f, 1), xyPad.getX() + i * (xyPad.getWidth() / 8) - 15, xyPad.getBottom() - 30, 30, 30, juce::Justification::centred, 1);
        g.drawFittedText(juce::String((i - 4) / 4.0f, 1), xyPad.getX() + i * (xyPad.getWidth() / 8) - 15, xyPad.getY() + 5, 30, 30, juce::Justification::centred, 1);
        g.drawFittedText(juce::String((4 - i) * 5), xyPad.getX() + 5, xyPad.getY() + i * (xyPad.getHeight() / 8) - 15, 30, 30, juce::Justification::centred, 1);
        g.drawFittedText(juce::String((4 - i) * 5), xyPad.getRight() - 30, xyPad.getY() + i * (xyPad.getHeight() / 8) - 15, 30, 30, juce::Justification::centred, 1);
    }
    g.setColour(juce::Colour::fromRGB(0xDE, 0x1C, 0x7C));
    g.drawRoundedRectangle(xyPad, 10, 2);

    // Draw thumbs
    leftChannelThumbPos = juce::Point<float>(juce::jmap<float>(leftChannelPanSlider.getValue(), leftChannelPanSlider.getMinimum(), leftChannelPanSlider.getMaximum(), xyPad.getX() + radius, xyPad.getRight() - radius),
        juce::jmap<float>(leftChannelGainSlider.getValue(), leftChannelGainSlider.getMaximum(), leftChannelGainSlider.getMinimum(), xyPad.getY() + radius, xyPad.getBottom() - radius));
    g.setColour(juce::Colour::fromRGB(0x00, 0x54, 0xC2));
    g.fillEllipse(leftChannelThumbPos.x - radius, leftChannelThumbPos.y - radius, radius * 2, radius * 2);
    g.setColour(juce::Colours::white);
    g.setFont(16);
    g.drawFittedText("L", leftChannelThumbPos.x - 4, leftChannelThumbPos.y - 5, 10, 10, juce::Justification::centred, 1);

    rightChannelThumbPos = juce::Point<float>(juce::jmap<float>(rightChannelPanSlider.getValue(), rightChannelPanSlider.getMinimum(), rightChannelPanSlider.getMaximum(), xyPad.getX() + radius, xyPad.getRight() - radius),
        juce::jmap<float>(rightChannelGainSlider.getValue(), rightChannelGainSlider.getMaximum(), rightChannelGainSlider.getMinimum(), xyPad.getY() + radius, xyPad.getBottom() - radius));
    g.setColour(juce::Colour::fromRGB(0x41, 0x92, 0x00));
    g.fillEllipse(rightChannelThumbPos.x - radius, rightChannelThumbPos.y - radius, radius * 2, radius * 2);
    g.setColour(juce::Colours::white);
    g.setFont(16);
    g.drawFittedText("R", rightChannelThumbPos.x - 4, rightChannelThumbPos.y - 5, 10, 10, juce::Justification::centred, 1);
}

void AetherAudioProcessorEditor::resized()
{
    xyPad.setBounds(10, 90, getWidth() - 20, getHeight() - 100);

    leftChannelPanSlider.setBounds(10, 10, 50, 50);
    rightChannelPanSlider.setBounds(340, 10, 50, 50);
    leftChannelGainSlider.setBounds(10, 40, 50, 50);
    rightChannelGainSlider.setBounds(340, 40, 50, 50);
}

void AetherAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
	if (leftChannelThumbPos.getDistanceFrom(juce::Point<float>(event.position.getX(), event.position.getY())) < radius) {
		draggedThumb = -1;
	}
	else if (rightChannelThumbPos.getDistanceFrom(juce::Point<float>(event.position.getX(), event.position.getY())) < radius) {
		draggedThumb = 1;
	}
    else {
		draggedThumb = 0;
	}
}

void AetherAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (draggedThumb != 0) {
        auto newPos = event.position;

        newPos.x = juce::jlimit<float>(xyPad.getX() + radius, xyPad.getRight() - radius, newPos.x);
        newPos.y = juce::jlimit<float>(xyPad.getY() + radius, xyPad.getBottom() - radius, newPos.y);

        if (draggedThumb == -1 ) {
            leftChannelPanSlider.setValue(juce::jmap<float>(newPos.x, xyPad.getX() + radius, xyPad.getRight() - radius, leftChannelPanSlider.getMinimum(), leftChannelPanSlider.getMaximum()));
            leftChannelGainSlider.setValue(juce::jmap<float>(newPos.y, xyPad.getY() + radius, xyPad.getBottom() - radius, leftChannelGainSlider.getMaximum(), leftChannelGainSlider.getMinimum()));
        }
		else if (draggedThumb == 1) {
			rightChannelPanSlider.setValue(juce::jmap<float>(newPos.x, xyPad.getX() + radius, xyPad.getRight() - radius, rightChannelPanSlider.getMinimum(), rightChannelPanSlider.getMaximum()));
            rightChannelGainSlider.setValue(juce::jmap<float>(newPos.y, xyPad.getY() + radius, xyPad.getBottom() - radius, rightChannelGainSlider.getMaximum(), rightChannelGainSlider.getMinimum()));
		}

        repaint();
    }
}