#pragma once

#include "JuceHeader.h"
#include "../Modules/Styles.h"
#include "../Modules/TextLabel.h"
#include "../Process/MainProcess.h"
#include "../Messaging/DataMessageListener.h"

//==============================================================================
class ChordNameComponent : public Component, public DataMessageListener
{
public:
    //==============================================================================
    ChordNameComponent (MainProcess&);
    ~ChordNameComponent();

    //==============================================================================
    void resized() override;

    //==============================================================================
    void handleNewMessage (const DataMessage* message) override;

private:
    //==============================================================================
    MainProcess& mMainProcess;
    GlobalState& mGlobalState;
    PresetState& mPresetState;
    MidiState& mMidiState;

    TextLabel mChordNameLabel;
    TextEditor mChordNameInput;

    void handleToggleMode (const DataMessage* message);
    void handlePresetFileNew (const DataMessage* message);
    void handlePresetFileLoaded (const DataMessage* message);
    void handleEditModeInputNote (const DataMessage* message);
    void handleEditModeOutputNotes (const DataMessage* message);
    void handleInputNoteOff (const DataMessage* message);
    void handleInputNoteOn (const DataMessage* message);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChordNameComponent)
};
