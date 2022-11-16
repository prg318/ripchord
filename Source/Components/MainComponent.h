#pragma once

#include "JuceHeader.h"
#include "../Modules/Styles.h"
#include "../Process/MainProcess.h"
#include "../Messaging/DataMessage.h"
#include "../Messaging/DataMessageListener.h"
#include "KeyboardViewComponent.h"
#include "PresetViewComponent.h"
#include "RightClickComponent.h"
#include "MenuComponent.h"

//==============================================================================
class MainComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    MainComponent (MainProcess&);
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    PresetState& mPresetState;

    KeyboardViewComponent mKeyboardViewComponent;
    PresetViewComponent mPresetViewComponent;
    RightClickComponent mRightClickComponent;
    MenuComponent mMenuComponent;

    Label mTitleLabel { "", "ripchord" };

    void handleToggleView (const DataMessage* message);
    void handleToggleMenu (const DataMessage* message);
    void handleToggleTheme (const DataMessage* message);
    void handleToggleRightClick (const DataMessage* message);
    void handlePresetFileNew (const DataMessage* message);
    void handlePresetFileLoaded (const DataMessage* message);

    void reconstruct();
    bool mShouldReconstruct = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
