#pragma once

#include "JuceHeader.h"
#include "../Modules/Images.h"
#include "../Modules/Styles.h"
#include "../Process/MainProcess.h"
#include "../Messaging/DataMessageListener.h"

//==============================================================================
class RightClickComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    RightClickComponent (MainProcess&);
    ~RightClickComponent();

    //==============================================================================
    void resized() override;

    //==============================================================================
    void mouseDown (const MouseEvent& event) override;

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    PresetState& mPresetState;

    Images mImages;
    DrawableButton mCutButton { "", DrawableButton::ImageStretched };
    DrawableButton mCopyButton { "", DrawableButton::ImageStretched };
    DrawableButton mPasteButton { "", DrawableButton::ImageStretched };
    DrawableButton mRightClickBg { "", DrawableButton::ImageStretched };

    //==============================================================================
    void paintWithCoordinates();

    //==============================================================================
    void handleToggleRightClick (const DataMessage* message);
    void handleToggleTheme (const DataMessage* message);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightClickComponent)
};
