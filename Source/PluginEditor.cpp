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
    panAttachment[0] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "leftChannelPan", panSlider[0]);
    gainAttachment[0] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "leftChannelGain", gainSlider[0]);
    panAttachment[1] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "rightChannelPan", panSlider[1]);
    gainAttachment[1] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "rightChannelGain", gainSlider[1]);

    panSlider[0].setSliderStyle(juce::Slider::RotaryVerticalDrag);
    panSlider[0].setRange(-1.0f, 1.0f, 0.01f);
    panSlider[0].setValue(audioProcessor.apvts.getRawParameterValue("leftChannelPan")->load());
    panSlider[0].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&panSlider[0]);
    panSlider[0].setEnabled(false);
    panSlider[0].setVisible(false);

    gainSlider[0].setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider[0].setRange(-20.0f, 20.0f, 0.01f);
    gainSlider[0].setValue(audioProcessor.apvts.getRawParameterValue("leftChannelGain")->load());
    gainSlider[0].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&gainSlider[0]);
    gainSlider[0].setEnabled(false);
    gainSlider[0].setVisible(false);

    panSlider[1].setSliderStyle(juce::Slider::RotaryVerticalDrag);
    panSlider[1].setRange(-1.0f, 1.0f, 0.01f);
    panSlider[1].setValue(audioProcessor.apvts.getRawParameterValue("rightChannelPan")->load());
    panSlider[1].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&panSlider[1]);
    panSlider[1].setEnabled(false);
    panSlider[1].setVisible(false);

    gainSlider[1].setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider[1].setRange(-20.0f, 20.0f, 0.01f);
    gainSlider[1].setValue(audioProcessor.apvts.getRawParameterValue("rightChannelGain")->load());
    gainSlider[1].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&gainSlider[1]);
    gainSlider[1].setEnabled(false);
    gainSlider[1].setVisible(false);
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

    // Draw title
    g.setColour(juce::Colour::fromRGB(0xDE, 0x1C, 0x7C));
    g.setFont(juce::Font(customTypeface)); 
    g.setFont(36);
    g.drawFittedText("Aether", getWidth() / 2.0f - 36, 40 - 18, 72, 36, juce::Justification::centred, 1);

    // Draw grid
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
    // Draw XY Pad
    g.setColour(juce::Colour::fromRGB(0xDE, 0x1C, 0x7C));
    g.drawRoundedRectangle(xyPad, 10, 2);

    // Draw thumbs for left and right channels
    thumbPos[0] = juce::Point<float>(juce::jmap<float>(panSlider[0].getValue(), panSlider[0].getMinimum(), panSlider[0].getMaximum(), xyPad.getX() + thumbRadius, xyPad.getRight() - thumbRadius),
        juce::jmap<float>(gainSlider[0].getValue(), gainSlider[0].getMaximum(), gainSlider[0].getMinimum(), xyPad.getY() + thumbRadius, xyPad.getBottom() - thumbRadius));
    g.setColour(juce::Colour::fromRGB(0x00, 0x54, 0xC2));
    g.fillEllipse(thumbPos[0].x - thumbRadius, thumbPos[0].y - thumbRadius, thumbRadius * 2, thumbRadius * 2);
    g.setColour(juce::Colours::white);
    g.setFont(16);
    g.drawFittedText("L", thumbPos[0].x - 4, thumbPos[0].y - 5, 10, 10, juce::Justification::centred, 1);

    thumbPos[1] = juce::Point<float>(juce::jmap<float>(panSlider[1].getValue(), panSlider[1].getMinimum(), panSlider[1].getMaximum(), xyPad.getX() + thumbRadius, xyPad.getRight() - thumbRadius),
        juce::jmap<float>(gainSlider[1].getValue(), gainSlider[1].getMaximum(), gainSlider[1].getMinimum(), xyPad.getY() + thumbRadius, xyPad.getBottom() - thumbRadius));
    g.setColour(juce::Colour::fromRGB(0x41, 0x92, 0x00));
    g.fillEllipse(thumbPos[1].x - thumbRadius, thumbPos[1].y - thumbRadius, thumbRadius * 2, thumbRadius * 2);
    g.setColour(juce::Colours::white);
    g.setFont(16);
    g.drawFittedText("R", thumbPos[1].x - 4, thumbPos[1].y - 5, 10, 10, juce::Justification::centred, 1);
}

void AetherAudioProcessorEditor::resized()
{
    xyPad.setBounds(10, 90, getWidth() - 20, getHeight() - 100);

    panSlider[0].setBounds(10, 10, 50, 50);
    gainSlider[0].setBounds(10, 40, 50, 50);
    panSlider[1].setBounds(340, 10, 50, 50);
    gainSlider[1].setBounds(340, 40, 50, 50);
}

void AetherAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
	if (thumbPos[0].getDistanceFrom(juce::Point<float>(event.position.getX(), event.position.getY())) < thumbRadius) {
		draggedThumb = 0;
	}
	else if (thumbPos[1].getDistanceFrom(juce::Point<float>(event.position.getX(), event.position.getY())) < thumbRadius) {
		draggedThumb = 1;
	}
    else {
		draggedThumb = -1;
	}
}

void AetherAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (draggedThumb != -1) {
        auto newPos = event.position;

        newPos.x = juce::jlimit<float>(xyPad.getX() + thumbRadius, xyPad.getRight() - thumbRadius, newPos.x);
        newPos.y = juce::jlimit<float>(xyPad.getY() + thumbRadius, xyPad.getBottom() - thumbRadius, newPos.y);

        if (draggedThumb == 0) {
            panSlider[0].setValue(juce::jmap<float>(newPos.x, xyPad.getX() + thumbRadius, xyPad.getRight() - thumbRadius, panSlider[0].getMinimum(), panSlider[0].getMaximum()));
            gainSlider[0].setValue(juce::jmap<float>(newPos.y, xyPad.getY() + thumbRadius, xyPad.getBottom() - thumbRadius, gainSlider[0].getMaximum(), gainSlider[0].getMinimum()));
        }
		else if (draggedThumb == 1) {
			panSlider[1].setValue(juce::jmap<float>(newPos.x, xyPad.getX() + thumbRadius, xyPad.getRight() - thumbRadius, panSlider[1].getMinimum(), panSlider[1].getMaximum()));
            gainSlider[1].setValue(juce::jmap<float>(newPos.y, xyPad.getY() + thumbRadius, xyPad.getBottom() - thumbRadius, gainSlider[1].getMaximum(), gainSlider[1].getMinimum()));
		}

        repaint();
    }
}