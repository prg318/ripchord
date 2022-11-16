#pragma once

#include "JuceHeader.h"
#include "../Modules/Images.h"
#include "../Modules/Styles.h"
#include "../Process/MainProcess.h"
#include "TagBrowserComponent.h"
#include "../Messaging/DataMessageListener.h"

//==============================================================================
class TagManagerComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    TagManagerComponent (MainProcess&);
    ~TagManagerComponent();

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
    BrowserState& mBrowserState;

    Viewport mTagViewport;
    TextEditor mNewTagInput;
    TagBrowserComponent mTagBrowser;

    Images mImages;
    DrawableButton mNewTagBg { "", DrawableButton::ImageStretched };
    DrawableButton mBackground { "", DrawableButton::ImageStretched };
    DrawableButton mTagBrowserBg { "", DrawableButton::ImageStretched };
    DrawableButton mCreateTagButton { "", DrawableButton::ImageStretched };

    //==============================================================================
    void handleToggleTagManager (const DataMessage* message);
    void handleTagCreated (const DataMessage* message);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TagManagerComponent)
};
