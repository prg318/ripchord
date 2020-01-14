#pragma once

#include "JuceHeader.h"
#include "Styles.h"

//==============================================================================
// The juce text editor won't draw the 'textWhenEmpty' centred, so we inherit it here to add that ability.
struct TextInput : public TextEditor
{
    using TextEditor::TextEditor;

    void paintOverChildren (Graphics& inGraphics) override
    {
        auto text = getTextToShowWhenEmpty();
        if (text.isNotEmpty() && (!hasKeyboardFocus (false)) && getTotalNumChars() == 0)
        {
            inGraphics.setColour (COLOR_GREY_MEDIUM);
            inGraphics.setFont (getFont());
            inGraphics.drawText (text, getLocalBounds(), Justification::centred, true);
        }

        getLookAndFeel().drawTextEditorOutline (inGraphics, getWidth(), getHeight(), *this);
    }
};
