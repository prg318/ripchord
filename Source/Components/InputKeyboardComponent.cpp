#include "InputKeyboardComponent.h"

//==============================================================================
InputKeyboardComponent::InputKeyboardComponent (MainProcess& inMainProcess)
:   mMainProcess (inMainProcess),
    mGlobalState (mMainProcess.getGlobalState()),
    mPresetState (mMainProcess.getPresetState())
{
    mGlobalState.DataMessageBroadcaster::addListener (this, ListenerType::kSync);
    mPresetState.DataMessageBroadcaster::addListener (this, ListenerType::kSync);
}

InputKeyboardComponent::~InputKeyboardComponent()
{
}

//==============================================================================
void InputKeyboardComponent::resized()
{
    initKeyboard();

    for (int inputNote = mFirstKey; inputNote <= mLastKey; inputNote++)
    {
        auto keyComponent = mKeyComponents.at (inputNote);
        keyComponent->onMouseUp = [this](const int inInputNote) { handleMouseUp (inInputNote); };
        keyComponent->onMouseDown = [this](const int inInputNote) { handleMouseDown (inInputNote); };
    }
}

//==============================================================================
void InputKeyboardComponent::handleMouseUp (const int inInputNote)
{

}

void InputKeyboardComponent::handleMouseDown (const int inInputNote)
{
    if (mGlobalState.isEditMode()) { mPresetState.handleEditModeMouseDownOnInput (inInputNote); }
}

//==============================================================================
void InputKeyboardComponent::handleNewMessage (const DataMessage* inMessage)
{
    switch (inMessage->messageCode)
    {
        case (MessageCode::kModeUpdated): { handleModeUpdated (inMessage); } break;
        case (MessageCode::kEditModeInputNote): { handleEditModeInputNote (inMessage); } break;
        default: { } break;
    };
}

void InputKeyboardComponent::handleModeUpdated (const DataMessage* inMessage)
{
    const int editModeInputNote = mPresetState.getEditModeInputNote();
    juce::Array<int> mappedInputNotes = mPresetState.getMappedInputNotes();
    Colour markerColor = mGlobalState.isEditMode() ? COLOR_GREEN : COLOR_BLUE;

    if (editModeInputNote > 0)
    {
        auto keyComponent = mKeyComponents.at (editModeInputNote);
        keyComponent->setNoteAndMarkerColor (keyComponent->getDefaultColor (editModeInputNote));
        mPresetState.resetEditModeInputNote();
    }

    for (int& inputNote : mappedInputNotes)
    {
        auto keyComponent = mKeyComponents.at (inputNote);
        keyComponent->setNoteColor (keyComponent->getDefaultColor (inputNote));
        keyComponent->setMarkerColor (markerColor);
    }
}

void InputKeyboardComponent::handleEditModeInputNote (const DataMessage* inMessage)
{
    const int prevEditModeInputNote = inMessage->messageVar1;
    const int nextEditModeInputNote = inMessage->messageVar2;
    bool prevEditModeInputNoteHasMarker = inMessage->messageVar3;

    if (prevEditModeInputNote > 0)
    {
        auto keyComponent = mKeyComponents.at (prevEditModeInputNote);
        auto defaultColor = keyComponent->getDefaultColor (prevEditModeInputNote);

        if (prevEditModeInputNoteHasMarker) { keyComponent->setNoteColor (defaultColor); }
        else { keyComponent->setNoteAndMarkerColor (defaultColor); }
    }

    if (nextEditModeInputNote > 0)
    {
        auto keyComponent = mKeyComponents.at (nextEditModeInputNote);
        keyComponent->setNoteAndMarkerColor (COLOR_GREEN);
    }
}
