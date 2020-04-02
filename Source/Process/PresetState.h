#pragma once

#include "JuceHeader.h"
#include "Presets.h"
#include "DataMessage.h"
#include "DataMessageBroadcaster.h"

//==============================================================================
class PresetState : public DataMessageBroadcaster
{
public:
    //==============================================================================
    PresetState();
    ~PresetState();

    //==============================================================================
    String getName();
    bool isPresetValid();
    bool isPresetModified();
    void resetEditModeInputNote();
    const int getEditModeInputNote();
    juce::Array<int> getPresetInputNotes();

    //==============================================================================
    bool containsChord (const int inputNote);
    String getChordName (const int inputNote);
    juce::Array<int> getChordNotes (const int inputNote);

    //==============================================================================
    void handleEditModeMouseDownOnInput (const int inputNote);
    void handleEditModeMouseDownOnOutput (const int outputNote);

    //==============================================================================
    void handleChordNameTextChanged (String chordName);
    void handlePresetNameTextChanged (String presetName);

    //==============================================================================
    void handleMouseDownOnCut (const int inputNote);
    void handleMouseDownOnCopy (const int inputNote);
    void handleMouseDownOnPaste (const int inputNote);

    //==============================================================================
    void handleMouseDownOnNew();
    void handleMouseDownOnMidi();
    void handleMouseDownOnSave();
    void handleMouseDownOnImport();
    void handleMouseDownOnExport();
    void handleMouseDownOnEditLeft();
    void handleMouseDownOnEditRight();
    void handleMouseDownOnPreset (File presetFile);

    //==============================================================================
    XmlElement* exportPresetStateXml();
    void importPresetStateXml (XmlElement* presetStateXml);

private:
    //==============================================================================
    String mName;
    std::map<int, Chord> mChords;

    Chord mEmptyChord;
    Chord mClipboardChord;
    int mEditModeInputNote = 0;

    String mPresetFileName;
    bool mIsPresetModified = false;

    Chord getChord (const int inputNote);
    void setChord (const int inputNote, Chord chord);

    void loadMidiFile (File midiFile);
    void loadPresetFile (File presetFile);
    void resetPresetState();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetState)
};
