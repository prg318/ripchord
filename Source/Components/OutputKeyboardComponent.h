#pragma once

#include "JuceHeader.h"
#include "MainProcess.h"
#include "KeyboardComponent.h"
#include "DataMessageListener.h"

//==============================================================================
class OutputKeyboardComponent : public KeyboardComponent, public DataMessageListener
{
public:
    //==============================================================================
    OutputKeyboardComponent (MainProcess&);
    ~OutputKeyboardComponent();

    //==============================================================================
    void resized() override;

    //==============================================================================
    void handleMouseUp (const int noteNumber);
    void handleMouseDown (const int noteNumber);

    //==============================================================================
    void handlePlayModeMouseUp (const int noteNumber);
    void handlePlayModeMouseDown (const int noteNumber);

    //==============================================================================
    void handleEditModeMouseUp (const int noteNumber);
    void handleEditModeMouseDown (const int noteNumber);

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;
    void handleEditModeInputNote (const DataMessage* message);

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    PresetState& mPresetState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputKeyboardComponent)
};
