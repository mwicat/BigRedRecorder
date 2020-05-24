#pragma once

#include "PluginProcessor.h"
#include "PluginUtils.h"


class ThumbnailButton : public DrawableButton
{
public:
    ThumbnailButton (const String& name, const DrawableButton::ButtonStyle buttonStyle)
    : DrawableButton (name, buttonStyle)
    {
    }
    
    void setImagesFromThumbnail(Drawable* image, Colour colour)
    {
        auto normalImage = image->createCopy();
        normalImage->replaceColour(Colours::black, colour);
        
        auto overImage = image->createCopy();
        overImage->replaceColour(Colours::black, colour.brighter());
        
        auto downImage = image->createCopy();
        downImage->replaceColour(Colours::black, colour.darker());

        auto disabledImage = image->createCopy();
        disabledImage->replaceColour(Colours::black, Colours::darkgrey);

        auto onImage = image->createCopy();
        onImage->replaceColour(Colours::black, colour.darker());
        
        setImages(normalImage.get(),
                  overImage.get(),
                  downImage.get(),
                  disabledImage.get(),
                  onImage.get());
    }
};


class DragAndDropArea : public ThumbnailButton, public DragAndDropContainer
{
public:
    DragAndDropArea (): ThumbnailButton("DragArea", DrawableButton::ButtonStyle::ImageAboveTextLabel)
    {
        setColour(DrawableButton::backgroundOnColourId, findColour(DrawableButton::backgroundColourId));
        std::unique_ptr<Drawable> image = Drawable::createFromImageData (
                                                              BinaryData::filemusicline_svg,
                                                              BinaryData::filemusicline_svgSize);
        setImagesFromThumbnail(image.get(), Colours::lightgreen);
        setButtonText("Drag me!");
        setEnabled(false);
    }
    
    void setEnabled (bool shouldBeEnabled)
    {
        setMouseCursor(shouldBeEnabled ? MouseCursor::DraggingHandCursor : MouseCursor::NormalCursor);
        ThumbnailButton::setEnabled(shouldBeEnabled);
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
};


class RecordButton : public ThumbnailButton, public Timer
{
public:
    RecordButton (): ThumbnailButton("Record", DrawableButton::ButtonStyle::ImageAboveTextLabel)
    {
        setColour(DrawableButton::backgroundOnColourId, findColour(DrawableButton::backgroundColourId));
        std::unique_ptr<Drawable> image = Drawable::createFromImageData (
                                                              BinaryData::recordcircleline_svg,
                                                              BinaryData::recordcircleline_svgSize);
        setImagesFromThumbnail(image.get(), colour);
        setClickingTogglesState(true);
        stopRecording();
    }
    
    void stopRecording()
    {
        setButtonText("Record");
        stopTimer();
    }
    
    void startRecording()
    {
        setButtonText("Stop");
        startTimer(500);
    }

    void timerCallback() override
    {
        on = !on;
        setColour(DrawableButton::textColourOnId, on ? colour : Colours::white);
    }
    
private:
    bool on = false;
    Colour colour = Colour (0xffff5c5c);
};


class AudioRecordingComponent  : public Component
{
public:
    AudioRecordingComponent (RecorderAudioProcessor& p)
    : processor(p)
    {
        addAndMakeVisible (recordButton);
        addAndMakeVisible(dndArea);
        
        recordButton.onClick = [this]
        {
            if (processor.isRecording())
                stopRecording();
            else
                startRecording();
        };
        targetDir = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("BigRedRecorder");
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
        recordButton.setBounds(area.withTrimmedRight(halfW).reduced(5));
        dndArea.setBounds(area.withTrimmedLeft(halfW).reduced(5));
    }

private:
    RecorderAudioProcessor& processor;
    RecordButton recordButton;
    File lastRecording;
    DragAndDropArea dndArea;
    File targetDir;

    void startRecording()
    {
        lastRecording = targetDir.getNonexistentChildFile ("BigRedRecorder_audio", ".wav");
        processor.startRecording (lastRecording);
        recordButton.startRecording();
        dndArea.setEnabled(false);
    }

    void stopRecording()
    {
        processor.stop();
        recordButton.stopRecording();
        dndArea.setFile(lastRecording);
        dndArea.setEnabled(true);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecordingComponent)
};
