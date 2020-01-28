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
    void handleMouseClickOnNew();
    void handleMouseClickOnSave();
    void handleMouseClickOnImport();
    void handleMouseClickOnExport();
    void handleMouseClickOnPreset (File presetFile);

    //==============================================================================
    XmlElement* exportPresetStateXml();
    void importPresetStateXml (XmlElement* presetStateXml);

private:
    //==============================================================================
    String mName;
    std::map<int, Chord> mChords;

    Chord mEmptyChord;
    int mEditModeInputNote = 0;

    String mPresetFileName;
    bool mIsPresetModified = false;

    Chord getChord (const int inputNote);
    void setChord (const int inputNote, Chord chord);

    void loadPresetFile (File presetFile);
    void resetPresetState();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetState)
};
