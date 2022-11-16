#pragma once

#include "JuceHeader.h"
#include "../Process/MainProcess.h"
#include "TagComponent.h"
#include "../Messaging/DataMessageListener.h"

//==============================================================================
class TagBarComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    TagBarComponent (MainProcess&);
    ~TagBarComponent();

    //==============================================================================
    void setDimensions (int width, int height);

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    BrowserState& mBrowserState;

    float mTagHeight = 0.f;
    float mTagWidth = 0.f;
    float mSpaceHeight = 0.f;
    float mSpaceWidth = 0.f;

    void hardRefresh();
    void refreshBrowser();

    OwnedArray<TagComponent> mTagsToDelete;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TagBarComponent)
};
