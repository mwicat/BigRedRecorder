/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AudioRecordingComponent.h"

//==============================================================================
/**
*/
class RecorderAudioProcessorEditor  :
    public AudioProcessorEditor
{
public:
    RecorderAudioProcessorEditor (RecorderAudioProcessor&);
    ~RecorderAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RecorderAudioProcessor& processor;
    AudioRecordingComponent recordingComponent;
    TooltipWindow tooltipWindow { this, 500 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecorderAudioProcessorEditor)
};
