#pragma once

#include "JuceHeader.h"
#include "../Modules/Images.h"
#include "../Modules/Styles.h"
#include "DeleteComponent.h"

//==============================================================================
class TagComponent : public Component
{
public:
    //==============================================================================
    TagComponent (String name, String type, bool isSelected, bool isAssignable);
    ~TagComponent();

    //==============================================================================
    void paint (Graphics& graphics) override;
    void resized() override;

    //==============================================================================
    void mouseDown (const MouseEvent& event) override;

    //==============================================================================
    std::function<void (const String name)> onClick;
    std::function<void (const String name)> onDelete;

    //==============================================================================
    std::function<void (const String name)> onUpShift;
    std::function<void (const String name)> onDownShift;

private:
    //==============================================================================
    String mName;
    String mType;

    bool mIsSelected;
    bool mIsAssignable;

    Images mImages;
    Label mTagLabel;
    DeleteComponent mDeleteComponent;

    DrawableButton mUpButton { "", DrawableButton::ImageStretched };
    DrawableButton mDownButton { "", DrawableButton::ImageStretched };

    DrawableButton mTrashButton { "", DrawableButton::ImageStretched };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TagComponent)
};
