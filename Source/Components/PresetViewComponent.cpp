#include "PresetViewComponent.h"

//==============================================================================
PresetViewComponent::PresetViewComponent (MainProcess& inMainProcess)
:   mMainProcess (inMainProcess),
    mGlobalState (mMainProcess.getGlobalState()),
    mBrowserState (mMainProcess.getBrowserState()),
    mPresetBrowser (inMainProcess)
{
    mGlobalState.DataMessageBroadcaster::addListener (this, ListenerType::kSync);
    mBrowserState.DataMessageBroadcaster::addListener (this, ListenerType::kSync);

    setWantsKeyboardFocus (true);

    mImages.setDrawableButtonImages (mSearchBg, "SearchBg.svg");
    mImages.setDrawableButtonImages (mPresetBg, "PresetBg.svg");

    mPresetFilterInput.setWantsKeyboardFocus (true);
    mPresetFilterInput.setColour (TextEditor::backgroundColourId, COLOR_TRANSPARENT);
    mPresetFilterInput.setTextToShowWhenEmpty ("search presets...", COLOR_GREY);
    mPresetFilterInput.onReturnKey = [this]() { grabKeyboardFocus(); };

    mPresetFilterInput.onTextChange = [this]()
    {
        mBrowserState.handlePresetFilterTextChanged (mPresetFilterInput.getText());
    };

    mImages.setDrawableButtonImages (mPowerButton, "PowerON.svg");
    mImages.setDrawableButtonImages (mFavoritesButton, "Favorites.svg");
    mImages.setDrawableButtonImages (mKeyboardsButton, "Keyboards.svg");

    mFavoritesButton.setTriggeredOnMouseDown (true);
    mFavoritesButton.onClick = [this]() { mBrowserState.handleMouseDownOnFavorites(); };

    mKeyboardsButton.setTriggeredOnMouseDown (true);
    mKeyboardsButton.onClick = [this]() { mGlobalState.toggleView(); };

    mPresetViewport.setScrollBarsShown (true, false);
    mPresetViewport.setViewedComponent (&mPresetBrowser, false);

    addAndMakeVisible (mSearchBg);
    addAndMakeVisible (mPresetBg);
    addAndMakeVisible (mPresetFilterInput);
    addAndMakeVisible (mPresetViewport);
    addAndMakeVisible (mPowerButton);
    addAndMakeVisible (mFavoritesButton);
    addAndMakeVisible (mKeyboardsButton);
}

PresetViewComponent::~PresetViewComponent()
{
}

//==============================================================================
void PresetViewComponent::paint (Graphics& inGraphics)
{
    auto mainArea = getLocalBounds();
    mSearchBg.setBounds (Styles::getRelativeBounds (mainArea, SPACE, OUTPUT_KEYBOARD_BG_Y, KEYBOARD_BG_WIDTH, ITEM_HEIGHT));
    mPresetBg.setBounds (Styles::getRelativeBounds (mainArea, SPACE, PRESET_VIEWPORT_Y, KEYBOARD_BG_WIDTH, PRESET_LIST_HEIGHT));
}

void PresetViewComponent::resized()
{
    auto mainArea = getLocalBounds();

    mPresetFilterInput.setBounds (Styles::getRelativeBounds (mainArea, LEFT_BUTTON_X, OUTPUT_KEYBOARD_BG_Y, KEYBOARD_BG_WIDTH, ITEM_HEIGHT));
    mPowerButton.setBounds (Styles::getRelativeBounds (mainArea, POWER_BUTTON_X, HEADER_Y, ITEM_HEIGHT, ITEM_HEIGHT));
    mFavoritesButton.setBounds (Styles::getRelativeBounds (mainArea, LEFT_BUTTON_X, FOOTER_Y, BUTTON_WIDTH, ITEM_HEIGHT));
    mKeyboardsButton.setBounds (Styles::getRelativeBounds (mainArea, RIGHT_BUTTON_X, FOOTER_Y, BUTTON_WIDTH, ITEM_HEIGHT));

    int inputHeight = mPresetFilterInput.getHeight();
    float inputFontHeight = inputHeight * TEXT_INPUT_FONT_HEIGHT_RATIO;

    mPresetFilterInput.applyFontToAllText (Font (inputFontHeight));
    mPresetFilterInput.setIndents ((inputHeight * 0.4f), int ((inputHeight - inputFontHeight) * 0.5f));

    int scrollbarWidth = inputHeight * 0.6f;
    mPresetViewport.setScrollBarThickness (scrollbarWidth);

    auto viewportArea = Styles::getRelativeBounds (mainArea, SPACE + 1, PRESET_VIEWPORT_Y + 1, PRESET_VIEWPORT_WIDTH - 2, PRESET_VIEWPORT_HEIGHT - 2);
    mPresetViewport.setBounds (viewportArea);

    auto presetArea = Styles::getRelativeBounds (mainArea, SPACE, PRESET_VIEWPORT_Y, PRESET_LIST_WIDTH, PRESET_LIST_HEIGHT);
    mPresetBrowser.setBounds (presetArea);
    mPresetBrowser.setDimensions (presetArea.getWidth(), presetArea.getHeight());
}

//==============================================================================
void PresetViewComponent::handleNewMessage (const DataMessage* inMessage)
{
    switch (inMessage->messageCode)
    {
        case (MessageCode::kToggleView): { handleToggleView (inMessage); } break;
        case (MessageCode::kToggleFavorites): { handleToggleFavorites (inMessage); } break;
        case (MessageCode::kPresetFilterTextChanged): { handlePresetFilterTextChanged (inMessage); } break;
        default: { } break;
    };
}

//==============================================================================
void PresetViewComponent::handleToggleView (const DataMessage* inMessage)
{
    mImages.setDrawableButtonImages (mPowerButton, mGlobalState.isPowerOn() ? "PowerON.svg" : "Power.svg");
}

void PresetViewComponent::handleToggleFavorites (const DataMessage* inMessage)
{
    bool isFavoritesOn = inMessage->messageVar1;
    mImages.setDrawableButtonImages (mFavoritesButton, isFavoritesOn ? "FavoritesON.svg" : "Favorites.svg");
}

void PresetViewComponent::handlePresetFilterTextChanged (const DataMessage* inMessage)
{
    String filterText = inMessage->messageVar1;
    mPresetFilterInput.setText (filterText, dontSendNotification);
}
