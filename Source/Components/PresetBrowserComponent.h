#pragma once

#include "JuceHeader.h"
#include "../Process/MainProcess.h"
#include "PresetComponent.h"
#include "PresetTaggerComponent.h"
#include "../Messaging/DataMessageListener.h"

//==============================================================================
class PresetBrowserComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    PresetBrowserComponent (MainProcess&);
    ~PresetBrowserComponent();

    //==============================================================================
    void setDimensions (int width, int height);

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    PresetState& mPresetState;
    BrowserState& mBrowserState;

    float mPresetHeight = 0.f;
    float mPresetWidth = 0.f;
    float mSpaceHeight = 0.f;
    float mSpaceWidth = 0.f;

    void hardRefresh();
    void refreshBrowser();

    OwnedArray<PresetComponent> mPresetsToDelete;
    OwnedArray<PresetTaggerComponent> mPresetTaggersToDelete;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetBrowserComponent)
};
