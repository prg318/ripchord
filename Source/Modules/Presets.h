#pragma once

#include "System.h"

//==============================================================================
const String PRESET_EXTENSION = ".rpc";
const File PRESET_FOLDER = System::getUserDataPath (ProjectInfo::companyName,
                                                    ProjectInfo::projectName).getChildFile ("Presets");

//==============================================================================
struct Preset
{
    int indexValue;
    String fileName;
    bool isFavorite;
};

//==============================================================================
struct Chord
{
    String name;
    juce::Array<int> notes;
};

//==============================================================================
namespace Presets
{
    //==============================================================================
    static inline XmlElement* getPresetXmlFromChords (std::map<int, Chord> inChords)
    {
        XmlElement* presetXml = new XmlElement ("preset");

        for (const auto& pair : inChords)
        {
            StringArray chordNotes;
            juce::Array<int> sortedNotes = pair.second.notes;
            sortedNotes.sort();

            for (const int chordNote : sortedNotes)
            {
                chordNotes.add (String (chordNote));
            }

            XmlElement* inputXml = new XmlElement ("input");
            XmlElement* chordXml = new XmlElement ("chord");

            inputXml->setAttribute ("note", String (pair.first));
            chordXml->setAttribute ("name", pair.second.name);
            chordXml->setAttribute ("notes", chordNotes.joinIntoString (";"));

            inputXml->addChildElement (chordXml);
            presetXml->addChildElement (inputXml);
        }

        return presetXml;
    }

    static inline std::map<int, Chord> getChordsFromPresetXml (XmlElement* inPresetXml)
    {
        std::map<int, Chord> chords;
        String inputTagName = inPresetXml->getTagName() == "preset" ? "input" : "mapping";

        forEachXmlChildElementWithTagName (*inPresetXml, inputXml, inputTagName)
        {
            Chord chord;
            juce::Array<int> notes;
            int note = inputXml->getIntAttribute("note");
            XmlElement* chordXml = inputXml->getChildByName ("chord");
            String notesString = chordXml->getStringAttribute ("notes");
            StringArray notesSA = StringArray::fromTokens (notesString, ";", "");

            String name = chordXml->getStringAttribute ("name");
            for (String& note : notesSA) { notes.add (note.getIntValue()); }

            if (notes.size() > 0)
            {
                notes.sort();
                chord.name = name;
                chord.notes = notes;
                chords[note] = chord;
            }
        }

        return chords;
    }

    static inline bool isValidFileName (String inFileName)
    {
        const String valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 _-";

        for (int index = 0; index < inFileName.length(); index++)
        {
            if (valid.indexOfChar (inFileName[index]) < 0) { return false; }
        }

        return true;
    }

    static inline Array<File> getSortedPresetFiles()
    {
        Array<File> files = PRESET_FOLDER.findChildFiles (File::findFiles, false, "*" + PRESET_EXTENSION);
        files.sort();
        return files;
    }

    static inline int getWhiteNoteNumber (int inIndex)
    {
        std::map<int, int> whiteNoteNumbers;

        whiteNoteNumbers[0] = 21;
        whiteNoteNumbers[1] = 23;
        whiteNoteNumbers[2] = 24;
        whiteNoteNumbers[3] = 26;
        whiteNoteNumbers[4] = 28;
        whiteNoteNumbers[5] = 29;
        whiteNoteNumbers[6] = 31;
        whiteNoteNumbers[7] = 33;
        whiteNoteNumbers[8] = 35;
        whiteNoteNumbers[9] = 36;
        whiteNoteNumbers[10] = 38;
        whiteNoteNumbers[11] = 40;
        whiteNoteNumbers[12] = 41;
        whiteNoteNumbers[13] = 43;
        whiteNoteNumbers[14] = 45;
        whiteNoteNumbers[15] = 47;
        whiteNoteNumbers[16] = 48;
        whiteNoteNumbers[17] = 50;
        whiteNoteNumbers[18] = 52;
        whiteNoteNumbers[19] = 53;
        whiteNoteNumbers[20] = 55;
        whiteNoteNumbers[21] = 57;
        whiteNoteNumbers[22] = 59;
        whiteNoteNumbers[23] = 60;
        whiteNoteNumbers[24] = 62;
        whiteNoteNumbers[25] = 64;
        whiteNoteNumbers[26] = 65;
        whiteNoteNumbers[27] = 67;
        whiteNoteNumbers[28] = 69;
        whiteNoteNumbers[29] = 71;
        whiteNoteNumbers[30] = 72;
        whiteNoteNumbers[31] = 74;
        whiteNoteNumbers[32] = 76;
        whiteNoteNumbers[33] = 77;
        whiteNoteNumbers[34] = 79;
        whiteNoteNumbers[35] = 81;
        whiteNoteNumbers[36] = 83;
        whiteNoteNumbers[37] = 84;
        whiteNoteNumbers[38] = 86;
        whiteNoteNumbers[39] = 88;
        whiteNoteNumbers[40] = 89;
        whiteNoteNumbers[41] = 91;
        whiteNoteNumbers[42] = 93;
        whiteNoteNumbers[43] = 95;
        whiteNoteNumbers[44] = 96;
        whiteNoteNumbers[45] = 98;
        whiteNoteNumbers[46] = 100;
        whiteNoteNumbers[47] = 101;
        whiteNoteNumbers[48] = 103;
        whiteNoteNumbers[49] = 105;
        whiteNoteNumbers[50] = 107;
        whiteNoteNumbers[51] = 108;

        return whiteNoteNumbers.at (inIndex);
    }

    static inline String getNoteNameFromNumber (int inNoteNumber)
    {
        std::map<int, String> noteNames;

        noteNames[21] = "A0";
        noteNames[22] = "A#0";
        noteNames[23] = "B0";
        noteNames[24] = "C1";
        noteNames[25] = "C#1";
        noteNames[26] = "D1";
        noteNames[27] = "D#1";
        noteNames[28] = "E1";
        noteNames[29] = "F1";
        noteNames[30] = "F#1";
        noteNames[31] = "G1";
        noteNames[32] = "G#1";
        noteNames[33] = "A1";
        noteNames[34] = "A#1";
        noteNames[35] = "B1";
        noteNames[36] = "C2";
        noteNames[37] = "C#2";
        noteNames[38] = "D2";
        noteNames[39] = "D#2";
        noteNames[40] = "E2";
        noteNames[41] = "F2";
        noteNames[42] = "F#2";
        noteNames[43] = "G2";
        noteNames[44] = "G#2";
        noteNames[45] = "A2";
        noteNames[46] = "A#2";
        noteNames[47] = "B2";
        noteNames[48] = "C3";
        noteNames[49] = "C#3";
        noteNames[50] = "D3";
        noteNames[51] = "D#3";
        noteNames[52] = "E3";
        noteNames[53] = "F3";
        noteNames[54] = "F#3";
        noteNames[55] = "G3";
        noteNames[56] = "G#3";
        noteNames[57] = "A3";
        noteNames[58] = "A#3";
        noteNames[59] = "B3";
        noteNames[60] = "C4";
        noteNames[61] = "C#4";
        noteNames[62] = "D4";
        noteNames[63] = "D#4";
        noteNames[64] = "E4";
        noteNames[65] = "F4";
        noteNames[66] = "F#4";
        noteNames[67] = "G4";
        noteNames[68] = "G#4";
        noteNames[69] = "A4";
        noteNames[70] = "A#4";
        noteNames[71] = "B4";
        noteNames[72] = "C5";
        noteNames[73] = "C#5";
        noteNames[74] = "D5";
        noteNames[75] = "D#5";
        noteNames[76] = "E5";
        noteNames[77] = "F5";
        noteNames[78] = "F#5";
        noteNames[79] = "G5";
        noteNames[80] = "G#5";
        noteNames[81] = "A5";
        noteNames[82] = "A#5";
        noteNames[83] = "B5";
        noteNames[84] = "C6";
        noteNames[85] = "C#6";
        noteNames[86] = "D6";
        noteNames[87] = "D#6";
        noteNames[88] = "E6";
        noteNames[89] = "F6";
        noteNames[90] = "F#6";
        noteNames[91] = "G6";
        noteNames[92] = "G#6";
        noteNames[93] = "A6";
        noteNames[94] = "A#6";
        noteNames[95] = "B6";
        noteNames[96] = "C7";
        noteNames[97] = "C#7";
        noteNames[98] = "D7";
        noteNames[99] = "D#7";
        noteNames[100] = "E7";
        noteNames[101] = "F7";
        noteNames[102] = "F#7";
        noteNames[103] = "G7";
        noteNames[104] = "G#7";
        noteNames[105] = "A7";
        noteNames[106] = "A#7";
        noteNames[107] = "B7";
        noteNames[108] = "C8";

        return noteNames.at (inNoteNumber);
    }

    static inline String generateNameFromNotes (juce::Array<int> inNotes)
    {
        String chordName = "";

        for (int index = 0; index < inNotes.size(); index++)
        {
            if ((index + 1) == inNotes.size())
            {
                chordName << getNoteNameFromNumber (inNotes[index]);
            }
            else
            {
                chordName << getNoteNameFromNumber (inNotes[index]) << " ";
            }
        }

        return chordName;
    }

    static inline std::map<int, Chord> getChordsFromMidiFile (File inMidiFile)
    {
        int chordIndex = 23;
        juce::Array<int> currentNotes;
        juce::Array<String> chordNames;
        std::map<int, Chord> chords;

        MidiFile midiFile;
        FileInputStream midiFileStream (inMidiFile);
        midiFile.readFrom (midiFileStream);
        const MidiMessageSequence* midiTrack = midiFile.getTrack (0);

        for (MidiMessageSequence::MidiEventHolder* event : *midiTrack) {
            const MidiMessage message = event->message;

            if (message.isNoteOn())
            {
                currentNotes.add (message.getNoteNumber());
            }

            if (message.isNoteOff() && !currentNotes.isEmpty())
            {
                currentNotes.sort();
                const String chordName = generateNameFromNotes (currentNotes);

                if (!chordNames.contains (chordName))
                {
                    Chord chord;
                    chord.name = chordName;
                    chord.notes = currentNotes;
                    chordNames.add (chordName);
                    chords[getWhiteNoteNumber (chordIndex)] = chord;
                    chordIndex = chordIndex + 1;
                }

                currentNotes.clear();
            }
        }

        return chords;
    }
}
