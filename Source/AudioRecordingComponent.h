#pragma once

#include "PluginProcessor.h"
#include "PluginUtils.h"


class DragAndDropArea : public Component, public DragAndDropContainer
{
public:
    DragAndDropArea ()
    {
        setColour(Label::backgroundColourId, Colours::darkgrey);
        icon = std::unique_ptr<Drawable>(Drawable::createFromImageData (BinaryData::dragdropline_svg, BinaryData::dragdropline_svgSize));
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (inside ? Colours::white : Colours::lightgrey);
        
        g.setColour (Colours::black);
        g.setFont (14.0f);
        auto area = getLocalBounds();
        auto x = (area.getWidth() - iconSize) / 2;
        auto y = (area.getHeight() - iconSize) / 2;
        icon->drawWithin(g, Rectangle<float>(x, y, iconSize, iconSize), RectanglePlacement::Flags::centred, 1.0f);
        //g.drawImageAt (dnd_icon.get(), );
        g.drawFittedText (
                          "Drag from this area and drop recorded"
                          " audio into application of your choice",
                          area.withTrimmedBottom(10),
                          Justification::centredBottom,
                          10);
    }
    
    void mouseEnter (const MouseEvent& event) override
    {
        inside = true;
        setMouseCursor(MouseCursor::DraggingHandCursor);
        repaint();
    }

    void mouseExit (const MouseEvent& event) override
    {
        inside = false;
        repaint();
    }

    void mouseDown (const MouseEvent &event) override
    {
        if (!file.existsAsFile())
        {
            return;
        }
        StringArray files;
        files.add(file.getFullPathName());
        startDragging (file.getFileName(), this);
        DragAndDropContainer::performExternalDragDropOfFiles(files, false);
    }
    
    void setFile(File f)
    {
        file = f;
    }
    
private:
    File file;
    std::unique_ptr<Drawable> icon;
    int iconSize = 64;
    bool inside = false;
};

class RecordButton : public TextButton, public Timer
{
public:
    RecordButton (): TextButton()
    {
        stopRecording();
    }
    
    void stopRecording()
    {
        stopTimer();
        setButtonText ("Record");
        setColour (TextButton::buttonColourId, Colour (0xffff5c5c));
        setColour (TextButton::textColourOnId, Colours::black);
    }
    
    void startRecording()
    {
        setButtonText ("Stop");
        startTimer(500);
    }

    void timerCallback() override
    {
        setColour(TextButton::buttonColourId, Colour (on ? 0xffff5c5c : 0xffca837c));
        on = !on;
    }
    
private:
    bool on = false;
};

//==============================================================================
class AudioRecordingComponent  : public Component
{
public:
    AudioRecordingComponent (RecorderAudioProcessor& p)
    : processor(p)
    {
        setOpaque (true);

        addAndMakeVisible(headerLabel);
        headerLabel.setJustificationType(Justification::centred);
        headerLabel.setFont(24.0f);
        headerLabel.setColour(Label::textColourId, Colour (0xffff5c5c));

        footerLabel.setJustificationType(Justification::bottomRight);
        footerLabel.setColour(Label::textColourId, Colour (0xffdab872));
        footerLabel.setFont (12.0f);
        
        addAndMakeVisible (recordButton);
        addAndMakeVisible(dndArea);
        addAndMakeVisible(footerLabel);
        addAndMakeVisible(hintLabel);
        
        recordButton.onClick = [this]
        {
            if (processor.isRecording())
                stopRecording();
            else
                startRecording();
        };
        
        targetDir = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("BigRedRecorder");
        
        hintLabel.setFont(14.0f);
        hintLabel.setText("Target directory: " + targetDir.getFullPathName(), NotificationType::dontSendNotification);
        handleResult(targetDir.createDirectory(), "creating " + targetDir.getFullPathName());
    }

    ~AudioRecordingComponent() override
    {
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colour (0xff1d1d33));
    }

    void resized() override
    {
        auto area = getLocalBounds();
        auto halfW = area.getWidth() / 2;
        headerLabel.setBounds(area.removeFromTop(40).reduced(5));
        footerLabel.setBounds(area.removeFromBottom(footerLabel.getFont().getHeight()+10).reduced(5));
        hintLabel.setBounds(area.removeFromBottom(hintLabel.getFont().getHeight()));
        recordButton.setBounds(area.withTrimmedRight(halfW).reduced(5));
        dndArea.setBounds(area.withTrimmedLeft(halfW).reduced(5));
    }

private:
    RecorderAudioProcessor& processor;
    Label headerLabel { {}, "Big Red Recorder" };
    Label footerLabel { {}, "mwicat"};
    Label hintLabel { {}, ""};
    RecordButton recordButton;
    File lastRecording;
    DragAndDropArea dndArea;
    File targetDir;

    void startRecording()
    {
        if (lastRecording.existsAsFile())
        {
            lastRecording.deleteFile();
        }
        lastRecording = targetDir.getNonexistentChildFile ("BigRedRecorder_audio", ".wav");
        dndArea.setFile(lastRecording);
        processor.startRecording (lastRecording);
        recordButton.startRecording();
    }

    void stopRecording()
    {
        processor.stop();
        lastRecording = File();
        recordButton.stopRecording();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecordingComponent)
};
