#pragma once

#include "JuceHeader.h"
#include "Keyboard.h"
#include "Styles.h"

//==============================================================================
class KeyComponent : public Component
{
public:
    //==============================================================================
    KeyComponent (const int noteNumber);
    ~KeyComponent();

    //==============================================================================
    void paint (Graphics& graphics) override;

    //==============================================================================
    void mouseUp (const MouseEvent& event) override;
    void mouseDown (const MouseEvent& event) override;

    //==============================================================================
    std::function<void (const int noteNumber)> onMouseUp;
    std::function<void (const int noteNumber)> onMouseDown;
    std::function<void (const int noteNumber, const MouseEvent& event)> onRightClick;

    //==============================================================================
    void setNoteColor (Colour color);
    void setMarkerColor (Colour color);
    void setNoteAndMarkerColor (Colour color);

    //==============================================================================
    const int getNoteNumber();
    Colour getDefaultColor();

private:
    //==============================================================================
    const int mNoteNumber;

    Colour mNoteColor;
    Colour mMarkerColor;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyComponent)
};
