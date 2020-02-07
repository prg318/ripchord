#include "MainProcess.h"

//==============================================================================
MainProcess::MainProcess()
{
}

MainProcess::~MainProcess()
{
}

//==============================================================================
void MainProcess::handleMidiBuffer (MidiBuffer& inMidiBuffer, int inNumberOfSamples)
{
    if (mMouseClickBuffer.getNumEvents() > 0)
    {
        transformMidiBuffer (mMouseClickBuffer);
        mMouseClickBuffer.clear();
    }
    else
    {
        transformMidiBuffer (inMidiBuffer);
    }

    inMidiBuffer.clear();
    inMidiBuffer.swapWith (mTransformedMidiBuffer);
}

//==============================================================================
void MainProcess::handlePlayModeMouseUpOnInput (int inInputNote)
{
    mMouseClickBuffer.addEvent (MidiMessage::noteOff (1, inInputNote, 0.8f), 0);
}

void MainProcess::handlePlayModeMouseDownOnInput (int inInputNote)
{
    mMouseClickBuffer.addEvent (MidiMessage::noteOn (1, inInputNote, 0.8f), 0);
}

//==============================================================================
void MainProcess::transformMidiBuffer (MidiBuffer& inMidiBuffer)
{
    int time;
    MidiMessage message;
    mTransformedMidiBuffer.clear();

    for (MidiBuffer::Iterator index (inMidiBuffer); index.getNextEvent (message, time);)
    {
        if (mGlobalState.isPlayMode() &&
            mControlsState.isTransposeOn() &&
            mControlsState.isTransposeNote (message.getNoteNumber()))
        {
            if (message.isNoteOn()) { handleActiveTransposeNote (message.getNoteNumber()); }
        }
        else
        {
            if (message.isNoteOn()) { handleNoteOn (message, time); }
            if (message.isNoteOff()) { handleNoteOff (message, time); }
            if (!message.isNoteOnOrOff()) { handleNonNote (message, time); }
        }
    }
}

void MainProcess::handleNoteOn (MidiMessage& inMessage, int inTime)
{
    int inChannel = inMessage.getChannel();
    int inInputNote = inMessage.getNoteNumber();
    float inVelocity = inMessage.getFloatVelocity();
    juce::Array<int> currentlyOnInputNotes = mMidiState.getCurrentlyOnInputNotes();
    std::map<int, Output> currentlyOnOutputNotes = mMidiState.getCurrentlyOnOutputNotes();

    currentlyOnInputNotes.addIfNotAlreadyThere (inInputNote);

    if (mPresetState.containsChord (inInputNote))
    {
        juce::Array<int> chordNotes = mPresetState.getChordNotes (inInputNote);

        for (int index = 0; index < chordNotes.size(); index++)
        {
            int chordNote = chordNotes[index];
            float delay = getChordNoteDelay (index);
            float velocity = getChordNoteVelocity (index, inVelocity);

            noteOnToOutputNote (inChannel, inTime, inInputNote, velocity, delay,
                                chordNote, currentlyOnOutputNotes, true);
        }
    }
    else
    {
        noteOnToOutputNote (inChannel, inTime, inInputNote, inVelocity, 0.f,
                            inInputNote, currentlyOnOutputNotes, false);
    }

    mMidiState.setCurrentlyOnInputNotes (currentlyOnInputNotes);
    mMidiState.setCurrentlyOnOutputNotes (currentlyOnOutputNotes);
}

void MainProcess::handleNoteOff (MidiMessage& inMessage, int inTime)
{
    int inChannel = inMessage.getChannel();
    int inInputNote = inMessage.getNoteNumber();
    float inVelocity = inMessage.getFloatVelocity();
    juce::Array<int> currentlyOnInputNotes = mMidiState.getCurrentlyOnInputNotes();
    std::map<int, Output> currentlyOnOutputNotes = mMidiState.getCurrentlyOnOutputNotes();

    currentlyOnInputNotes.removeFirstMatchingValue (inInputNote);

    if (mPresetState.containsChord (inInputNote))
    {
        juce::Array<int> chordNotes = mPresetState.getChordNotes (inInputNote);

        for (int index = 0; index < chordNotes.size(); index++)
        {
            int chordNote = chordNotes[index];
            float delay = getChordNoteDelay (index);
            float velocity = getChordNoteVelocity (index, inVelocity);

            noteOffToOutputNote (inChannel, inTime, inInputNote, velocity, delay,
                                 chordNote, currentlyOnOutputNotes, true);
        }
    }
    else
    {
        noteOffToOutputNote (inChannel, inTime, inInputNote, inVelocity, 0.f,
                             inInputNote, currentlyOnOutputNotes, false);
    }

    mMidiState.setCurrentlyOnInputNotes (currentlyOnInputNotes);
    mMidiState.setCurrentlyOnOutputNotes (currentlyOnOutputNotes);
}

void MainProcess::handleNonNote (MidiMessage& inMessage, int inTime)
{
    mTransformedMidiBuffer.addEvent (inMessage, inTime);
}

//==============================================================================
float MainProcess::getChordNoteDelay (int inIndex)
{
    float delay = 0.f;
    float delayDepth = mControlsState.getDelayDepth() / 100000.f;
    float delayVariance = mControlsState.getDelayVariance() / 100000.f;

    if (delayDepth > 0.f)
    {
        DBG ("delayDepth: " << delayDepth);
    }

    if (delayVariance > 0.f)
    {
        DBG ("delayVariance: " << delayVariance);
    }

    return delay;
}

float MainProcess::getChordNoteVelocity (int inIndex, float inVelocity)
{
    float velocity = inVelocity;
    float velocityDepth = mControlsState.getVelocityDepth() / 100000.f;
    float velocityVariance = mControlsState.getVelocityVariance() / 100000.f;

    if (velocityDepth > 0.f)
    {
        DBG ("velocityDepth: " << velocityDepth);
    }

    if (velocityVariance > 0.f)
    {
        DBG ("velocityVariance: " << velocityVariance);
    }

    return velocity;
}

//==============================================================================
void MainProcess::noteOnToOutputNote (int inChannel, int inTime, int inInputNote, float inVelocity, float inDelay,
                                      int inOutputNote, std::map<int, Output>& inCurrentlyOnOutputNotes, bool inIsChord)
{
    const int outputNote = inIsChord && mGlobalState.isPlayMode() ?
    mControlsState.getTransposedNote (inOutputNote, mControlsState.getActiveTransposeNote()) : inOutputNote;
    const int triggerCount = mMidiState.getOutputNoteTriggerCount (outputNote);

    if (triggerCount == 1)
    {
        const auto& message = MidiMessage::noteOff (inChannel, outputNote, inVelocity);
        mTransformedMidiBuffer.addEvent (message, inTime);

        auto pair = inCurrentlyOnOutputNotes.find (outputNote);
        juce::Array<int> triggers = pair->second.triggers;
        triggers.add (inInputNote);
        pair->second.triggers = triggers;
    }

    if (triggerCount == 0)
    {
        juce::Array<int> triggers;
        triggers.add (inInputNote);
        inCurrentlyOnOutputNotes[outputNote].triggers = triggers;
    }

    if (triggerCount == 1 || triggerCount == 0)
    {
        const auto& message = MidiMessage::noteOn (inChannel, outputNote, inVelocity);
        mTransformedMidiBuffer.addEvent (message, inTime);
    }
}

void MainProcess::noteOffToOutputNote (int inChannel, int inTime, int inInputNote, float inVelocity, float inDelay,
                                       int inOutputNote, std::map<int, Output>& inCurrentlyOnOutputNotes, bool inIsChord)
{
    const int outputNote = inIsChord && mGlobalState.isPlayMode() ?
    mControlsState.getTransposedNote (inOutputNote, mControlsState.getActiveTransposeNote()) : inOutputNote;
    bool containsTrigger = mMidiState.containsOutputNoteTrigger (outputNote, inInputNote);
    const int triggerCount = mMidiState.getOutputNoteTriggerCount (outputNote);

    if (triggerCount == 2 && containsTrigger)
    {
        auto pair = inCurrentlyOnOutputNotes.find (outputNote);
        juce::Array<int> triggers = pair->second.triggers;
        triggers.removeFirstMatchingValue (inInputNote);
        pair->second.triggers = triggers;
    }

    if (triggerCount == 1 && containsTrigger)
    {
        const auto& message = MidiMessage::noteOff (inChannel, outputNote, inVelocity);
        mTransformedMidiBuffer.addEvent (message, inTime);
        inCurrentlyOnOutputNotes.erase (outputNote);
    }
}

//==============================================================================
void MainProcess::handleActiveTransposeNote (int inInputNote)
{
    if (inInputNote == mControlsState.getTransposeBase() + 12) { return; }
    if (mMidiState.getCurrentlyOnInputNotes().size() > 0) { return; }

    else if (mControlsState.getActiveTransposeNote() != inInputNote)
    {
        mMidiState.setActiveTransposeNoteIfAllowed (inInputNote);
    }

    else if (mControlsState.getActiveTransposeNote() == inInputNote)
    {
        mMidiState.setActiveTransposeNoteIfAllowed (-1);
    }
}

//==============================================================================
XmlElement* MainProcess::exportSessionXml()
{
    XmlElement* sessionXml = new XmlElement ("Session");
    sessionXml->addChildElement (mControlsState.exportControlsStateXml());
    sessionXml->addChildElement (mPresetState.exportPresetStateXml());
    return sessionXml;
}

void MainProcess::importSessionXml (XmlElement* inSessionXml)
{
    if (inSessionXml->getTagName() != "Session") { return; }

    XmlElement* controlsStateXml = inSessionXml->getChildByName("ControlsState");
    XmlElement* presetStateXml = inSessionXml->getChildByName("PresetState");

    mControlsState.importControlsStateXml (controlsStateXml);
    mPresetState.importPresetStateXml (presetStateXml);
}
