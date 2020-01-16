#pragma once

#include "JuceHeader.h"
#include "Images.h"
#include "Styles.h"
#include "MainProcess.h"
#include "DataMessageListener.h"

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
    std::function<void()> handleBackgroundClick;

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    PresetState& mPresetState;

    Images mImages;
    DrawableButton mNewButton { "", DrawableButton::ImageStretched };
    DrawableButton mImportButton { "", DrawableButton::ImageStretched };
    DrawableButton mExportButton { "", DrawableButton::ImageStretched };
    DrawableButton mCommunityButton { "", DrawableButton::ImageStretched };

    //==============================================================================
    void handleMenuUpdated (const DataMessage* message);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};
