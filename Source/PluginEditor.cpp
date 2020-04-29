/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RecorderAudioProcessorEditor::RecorderAudioProcessorEditor (RecorderAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), recordingComponent (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 300);
    addAndMakeVisible(recordingComponent);
}

RecorderAudioProcessorEditor::~RecorderAudioProcessorEditor()
{
}

//==============================================================================
void RecorderAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (30.05f);
}

void RecorderAudioProcessorEditor::resized()
{
    recordingComponent.setBounds(getLocalBounds());
}
