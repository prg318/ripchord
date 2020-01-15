#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent (MainProcess& inMainProcess)
:   mMainProcess (inMainProcess),
    mGlobalState (mMainProcess.getGlobalState()),
    mPresetState (mMainProcess.getPresetState()),
    mKeyboardViewComponent (inMainProcess),
    mPresetViewComponent (inMainProcess),
    mMenuComponent (inMainProcess)
{
    mGlobalState.DataMessageBroadcaster::addListener (this, ListenerType::kSync);
    mPresetState.DataMessageBroadcaster::addListener (this, ListenerType::kSync);

    setOpaque (true);
    mTitleLabel.setFont (Font().boldened());
    mTitleLabel.setColour (Label::textColourId, COLOR_PURPLE);

    mImages.setDrawableButtonImages (mMenuButton, "Gear.svg");
    mMenuButton.setTriggeredOnMouseDown (true);
    mMenuButton.onClick = [this]() { mGlobalState.toggleMenu(); };
    mMenuComponent.handleBackgroundClick = [this]() { mGlobalState.toggleMenu(); };

    addAndMakeVisible (mTitleLabel);
    addAndMakeVisible (mKeyboardViewComponent);
    addChildComponent (mPresetViewComponent);

    addAndMakeVisible (mMenuButton);
    addChildComponent (mMenuComponent);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& inGraphics)
{
    inGraphics.setColour (COLOR_GREY_LIGHTEST);
    inGraphics.fillRect (getLocalBounds());
}

void MainComponent::resized()
{
    auto mainArea = getLocalBounds();
    mKeyboardViewComponent.setBounds (mainArea);
    mPresetViewComponent.setBounds (mainArea);
    mMenuComponent.setBounds (mainArea);

    auto titleArea = Styles::getRelativeBounds (mainArea, SPACE - 4, HEADER_Y, BUTTON_WIDTH + 10, ITEM_HEIGHT - 2);
    mTitleLabel.setFont (Font ((float) titleArea.getHeight()).boldened());
    mTitleLabel.setBounds (titleArea);

    auto menuButtonArea = Styles::getRelativeBounds (mainArea, MENU_X, HEADER_Y, ITEM_HEIGHT, ITEM_HEIGHT);
    mMenuButton.setBounds (menuButtonArea);
}

//==============================================================================
void MainComponent::handleNewMessage (const DataMessage* inMessage)
{
    switch (inMessage->messageCode)
    {
        case (MessageCode::kViewUpdated): { handleViewUpdated (inMessage); } break;
        case (MessageCode::kMenuUpdated): { handleMenuUpdated (inMessage); } break;
        case (MessageCode::kPresetFileNew): { handlePresetFileNew (inMessage); } break;
        default: { } break;
    };
}

void MainComponent::handleViewUpdated (const DataMessage* inMessage)
{
    mKeyboardViewComponent.setVisible (!mGlobalState.isPresetView());
    mPresetViewComponent.setVisible (mGlobalState.isPresetView());
}

void MainComponent::handleMenuUpdated (const DataMessage* inMessage)
{
    mMenuComponent.setVisible (mGlobalState.isMenuVisible());
}

void MainComponent::handlePresetFileNew (const DataMessage* inMessage)
{
    if (mGlobalState.isPlayMode()) { mGlobalState.toggleMode(); }
    if (mGlobalState.isPresetView()) { mGlobalState.toggleView(); }
}
