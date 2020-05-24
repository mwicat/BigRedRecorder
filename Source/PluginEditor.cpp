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
    addAndMakeVisible(recordingComponent);
    double ratio = 2.0;
    int w = processor.lastUIWidth;
    int h = processor.lastUIHeight;
    setResizeLimits(150, 150/ratio, 1200, 1200/ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(w, h);
}

RecorderAudioProcessorEditor::~RecorderAudioProcessorEditor()
{
}

//==============================================================================
void RecorderAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void RecorderAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    processor.lastUIWidth = bounds.getWidth();
    processor.lastUIHeight = bounds.getHeight();
    recordingComponent.setBounds(getLocalBounds());
}
