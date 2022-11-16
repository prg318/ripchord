#pragma once

#include "JuceHeader.h"
#include "../Modules/Images.h"
#include "../Modules/Styles.h"
#include "../Modules/TextLabel.h"
#include "../Process/MainProcess.h"
#include "../Messaging/DataMessageListener.h"

//==============================================================================
class PresetNameComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    PresetNameComponent (MainProcess&);
    ~PresetNameComponent();

    //==============================================================================
    void resized() override;

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    PresetState& mPresetState;
    BrowserState& mBrowserState;
    MidiState& mMidiState;

    Images mImages;
    DrawableButton mLeftArrowButton { "", DrawableButton::ImageStretched };
    DrawableButton mRightArrowButton { "", DrawableButton::ImageStretched };

    TextLabel mPresetNameLabel { "", "init" };
    TextEditor mPresetNameInput;

    void handleToggleMode (const DataMessage* message);
    void handlePresetFileNew (const DataMessage* message);
    void handlePresetNameTextChanged (const DataMessage* message);
    void handleCurrentIndexChanged (const DataMessage* message);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetNameComponent)
};
