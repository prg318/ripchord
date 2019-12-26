#include "KeyComponent.h"

//==============================================================================
KeyComponent::KeyComponent (const int inNoteNumber)
:   mNoteNumber (inNoteNumber)
{
    bool isBlackKey = Interface::isBlackKey (mNoteNumber);
    Colour defaultColor = isBlackKey ? COLOR_GREY_DARK : COLOR_WHITE;

    mNoteColor = defaultColor;
    mMarkerColor = defaultColor;
}

KeyComponent::~KeyComponent()
{
}

//==============================================================================
void KeyComponent::paint (Graphics& inGraphics)
{
    const int markerHeight = 4;
    auto keyArea = getLocalBounds().reduced (1);
    auto noteHeight = keyArea.getHeight() - markerHeight;

    inGraphics.setColour (COLOR_GREY_LIGHT);
    inGraphics.fillAll();

    inGraphics.setColour (mNoteColor);
    inGraphics.fillRect (keyArea.removeFromTop (noteHeight));

    inGraphics.setColour (mMarkerColor);
    inGraphics.fillRect (keyArea.removeFromTop (markerHeight));
}

//==============================================================================
const int KeyComponent::getNoteNumber()
{
    return mNoteNumber;
}

//==============================================================================
void KeyComponent::mouseUp (const MouseEvent& inEvent)
{
    if (onMouseUp) { onMouseUp (mNoteNumber); }
}

void KeyComponent::mouseDown (const MouseEvent& inEvent)
{
    if (onMouseDown) { onMouseDown (mNoteNumber); }
}

//==============================================================================
void KeyComponent::setNoteColor (Colour inColor)
{
    mNoteColor = inColor;
    repaint();
}

void KeyComponent::setMarkerColor (Colour inColor)
{
    mMarkerColor = inColor;
    repaint();
}
