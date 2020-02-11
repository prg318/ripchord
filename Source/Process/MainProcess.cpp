#include "MainProcess.h"

//==============================================================================
MainProcess::MainProcess()
{
}

MainProcess::~MainProcess()
{
}

//==============================================================================
void MainProcess::handleMidiBuffer (MidiBuffer& inMidiBuffer)
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

    handleNoteEventQueu();

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
    int sampleNumber;
    MidiMessage message;
    mTransformedMidiBuffer.clear();

    for (MidiBuffer::Iterator index (inMidiBuffer); index.getNextEvent (message, sampleNumber);)
    {
        if (mGlobalState.isPlayMode() &&
            mControlsState.isTransposeOn() &&
            mControlsState.isTransposeNote (message.getNoteNumber()))
        {
            if (message.isNoteOn()) { handleActiveTransposeNote (message.getNoteNumber()); }
        }
        else
        {
            if (message.isNoteOn()) { handleNoteOn (message, sampleNumber); }
            if (message.isNoteOff()) { handleNoteOff (message, sampleNumber); }
            if (!message.isNoteOnOrOff()) { handleNonNote (message, sampleNumber); }
        }
    }
}

void MainProcess::handleNoteOn (MidiMessage& inMessage, int inSampleNumber)
{
    bool isNoteOn = true;
    int inChannel = inMessage.getChannel();
    int inInputNote = inMessage.getNoteNumber();
    float inVelocity = inMessage.getFloatVelocity();
    mMidiState.setInputNoteOn (inInputNote);

    if (mPresetState.containsChord (inInputNote))
    {
        juce::Array<int> chordNotes = mPresetState.getChordNotes (inInputNote);

        for (int index = 0; index < chordNotes.size(); index++)
        {
            float delayDepth = mControlsState.getDelayDepth();
            float delayVariance = mControlsState.getDelayVariance();
            int activeTransposeNote = mControlsState.getActiveTransposeNote();
            int transposedNote = mControlsState.getTransposedNote (chordNotes[index], activeTransposeNote);
            int chordNote = mGlobalState.isPlayMode() ? transposedNote : chordNotes[index];
            NoteEvent noteEvent { inChannel, inSampleNumber, inVelocity, inInputNote, chordNote, isNoteOn };

            if (mGlobalState.isEditMode() || index == 0 ||
                (delayDepth < MIN_DELAY_DEPTH && delayVariance < MIN_DELAY_VARIANCE))
            {
                sendOutputNoteOn (noteEvent);
            }
            else
            {
                mMidiState.addNoteEventToQueu (noteEvent, index, delayDepth, delayVariance);
            }
        }
    }
    else
    {
        NoteEvent noteEvent { inChannel, inSampleNumber, inVelocity, inInputNote, inInputNote, isNoteOn };
        sendOutputNoteOn (noteEvent);
    }
}

void MainProcess::handleNoteOff (MidiMessage& inMessage, int inSampleNumber)
{
    bool isNoteOn = false;
    int inChannel = inMessage.getChannel();
    int inInputNote = inMessage.getNoteNumber();
    float inVelocity = inMessage.getFloatVelocity();
    mMidiState.setInputNoteOff (inInputNote);

    if (mPresetState.containsChord (inInputNote))
    {
        juce::Array<int> chordNotes = mPresetState.getChordNotes (inInputNote);

        for (int index = 0; index < chordNotes.size(); index++)
        {
            float delayDepth = mControlsState.getDelayDepth();
            float delayVariance = mControlsState.getDelayVariance();
            int activeTransposeNote = mControlsState.getActiveTransposeNote();
            int transposedNote = mControlsState.getTransposedNote (chordNotes[index], activeTransposeNote);
            int chordNote = mGlobalState.isPlayMode() ? transposedNote : chordNotes[index];
            NoteEvent noteEvent { inChannel, inSampleNumber, inVelocity, inInputNote, chordNote, isNoteOn };

            if (mGlobalState.isEditMode() || index == 0 ||
                (delayDepth < MIN_DELAY_DEPTH && delayVariance < MIN_DELAY_VARIANCE))
            {
                sendOutputNoteOff (noteEvent);
            }
            else
            {
                mMidiState.addNoteEventToQueu (noteEvent, index, delayDepth, delayVariance);
            }
        }
    }
    else
    {
        NoteEvent noteEvent { inChannel, inSampleNumber, inVelocity, inInputNote, inInputNote, isNoteOn };
        sendOutputNoteOff (noteEvent);
    }
}

void MainProcess::handleNonNote (MidiMessage& inMessage, int inSampleNumber)
{
    mTransformedMidiBuffer.addEvent (inMessage, inSampleNumber);
}

//==============================================================================
void MainProcess::sendOutputNoteOn (NoteEvent inNoteEvent)
{
    juce::Array<int> triggers = mMidiState.getOutputNoteTriggers (inNoteEvent.outputNote);

    if (triggers.size() == 1)
    {
        const auto& message = MidiMessage::noteOff (inNoteEvent.channel, inNoteEvent.outputNote, inNoteEvent.velocity);
        mTransformedMidiBuffer.addEvent (message, inNoteEvent.sampleNumber);
    }

    if (triggers.size() < 2)
    {
        const auto& message = MidiMessage::noteOn (inNoteEvent.channel, inNoteEvent.outputNote, inNoteEvent.velocity);
        mTransformedMidiBuffer.addEvent (message, inNoteEvent.sampleNumber);

        triggers.add (inNoteEvent.inputNote);
        mMidiState.setOutputNoteOn (inNoteEvent.outputNote, triggers);
    }
}

void MainProcess::sendOutputNoteOff (NoteEvent inNoteEvent)
{
    juce::Array<int> triggers = mMidiState.getOutputNoteTriggers (inNoteEvent.outputNote);
    if (triggers.indexOf (inNoteEvent.inputNote) < 0) { return; }

    if (triggers.size() == 1)
    {
        const auto& message = MidiMessage::noteOff (inNoteEvent.channel, inNoteEvent.outputNote, inNoteEvent.velocity);
        mTransformedMidiBuffer.addEvent (message, inNoteEvent.sampleNumber);
    }

    if (triggers.size() <= 2)
    {
        triggers.removeFirstMatchingValue (inNoteEvent.inputNote);
        mMidiState.setOutputNoteOff (inNoteEvent.outputNote, triggers);
    }
}

//==============================================================================
void MainProcess::handleNoteEventQueu()
{
    if (mGlobalState.isEditMode()) { return; }

    for (NoteEvent& noteEvent : mMidiState.getNoteEventsToSend())
    {
        if (noteEvent.isNoteOn) { sendOutputNoteOn (noteEvent); }
        if (!noteEvent.isNoteOn) { sendOutputNoteOff (noteEvent); }
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
