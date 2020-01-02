#pragma once

#include "JuceHeader.h"
#include "GlobalState.h"
#include "PresetState.h"
#include "MidiState.h"

//==============================================================================
class MainProcess
{
public:
    //==============================================================================
    MainProcess();
    ~MainProcess();

    //==============================================================================
    void handleMidiBuffer (MidiBuffer& midiBuffer, int numberOfSamples);

    //==============================================================================
    GlobalState& getGlobalState() { return mGlobalState; }
    PresetState& getPresetState() { return mPresetState; }
    MidiState& getMidiState() { return mMidiState; }

private:
    //==============================================================================
    MidiBuffer mTransformedMidiBuffer;
    void transformMidiBuffer (MidiBuffer& midiBuffer);
    void handleNoteOn (MidiMessage message, int time);
    void handleNoteOff (MidiMessage message, int time);
    void handleNonNote (MidiMessage message, int time);

    //==============================================================================
    GlobalState mGlobalState;
    PresetState mPresetState;
    MidiState mMidiState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainProcess)
};
