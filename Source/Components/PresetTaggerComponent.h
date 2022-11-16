#pragma once

#include "JuceHeader.h"
#include "../Modules/Images.h"
#include "../Modules/Styles.h"
#include "../Modules/Presets.h"

//==============================================================================
class PresetTaggerComponent : public Component
{
public:
    //==============================================================================
    PresetTaggerComponent (Preset preset, bool isInAssignableTag);
    ~PresetTaggerComponent();

    //==============================================================================
    void paint (Graphics& graphics) override;
    void resized() override;
    void mouseDown (const MouseEvent& event) override;

    //==============================================================================
    std::function<void (const int indexValue)> onClick;

private:
    //==============================================================================
    Preset mPreset;
    bool mIsInAssignableTag;

    Images mImages;
    Label mPresetLabel;

    DrawableButton mCheckButton { "", DrawableButton::ImageStretched };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetTaggerComponent)
};
