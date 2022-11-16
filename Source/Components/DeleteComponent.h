#pragma once

#include "JuceHeader.h"
#include "../Modules/Images.h"
#include "../Modules/Styles.h"

//==============================================================================
class DeleteComponent : public Component
{
public:
    //==============================================================================
    DeleteComponent (String type);
    ~DeleteComponent();

    void paint (Graphics& graphics) override;
    void resized() override;

    //==============================================================================
    std::function<void()> onClick;

private:
    //==============================================================================
    String mType;

    Images mImages;
    DrawableButton mCancelButton { "", DrawableButton::ImageStretched };
    DrawableButton mDeleteButton { "", DrawableButton::ImageStretched };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeleteComponent)
};
