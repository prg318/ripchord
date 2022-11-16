#pragma once

#include "JuceHeader.h"
#include "../Modules/Images.h"
#include "../Modules/Styles.h"
#include "../Process/MainProcess.h"
#include "../Messaging/DataMessageListener.h"

//==============================================================================
class MenuComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    MenuComponent (MainProcess&);
    ~MenuComponent();

    //==============================================================================
    void paint (Graphics&) override;
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
    Label mCreditsLabel { "", "we support producers" };

    DrawableButton mThemeButton { "", DrawableButton::ImageStretched };
    DrawableButton mResetButton { "", DrawableButton::ImageStretched };
    DrawableButton mCreditsButton { "", DrawableButton::ImageStretched };

    DrawableButton mBackground { "", DrawableButton::ImageStretched };
    DrawableButton mNewPresetButton { "", DrawableButton::ImageStretched };
    DrawableButton mDuplicateButton { "", DrawableButton::ImageStretched };
    DrawableButton mImportMidiButton { "", DrawableButton::ImageStretched };
    DrawableButton mExportMidiButton { "", DrawableButton::ImageStretched };
    DrawableButton mImportPresetButton { "", DrawableButton::ImageStretched };
    DrawableButton mExportPresetButton { "", DrawableButton::ImageStretched };
    DrawableButton mImportMPCButton { "", DrawableButton::ImageStretched };
    DrawableButton mVersionButton { "", DrawableButton::ImageStretched };

    //==============================================================================
    void handleToggleMenu (const DataMessage* message);
    void handleToggleTheme (const DataMessage* message);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};
