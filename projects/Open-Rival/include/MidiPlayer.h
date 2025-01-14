#pragma once

#include <RtMidi.h>

#include <mutex>

#include "MidiFile.h"

namespace Rival {

class MidiPlayer
{
public:
    /**
     * Initializes the MIDI system.
     *
     * Throws a RtMidiError if a MIDI system initialization error occurs.
     */
    void init();

    /**
     * Plays the given MIDI file synchronously from start to finish.
     *
     * Throws an exception if the MidiPlayer is not initialized.
     */
    void play(MidiFile midi);

    /**
     * Stops the current track.
     *
     * This may block the calling thread for a few milliseconds as it has
     * to wait for the next scheduled MIDI command to finish.
     */
    void stop();

private:
    std::mutex playingMutex;
    RtMidiOut midiOut;
    bool stopped { true };
};

}  // namespace Rival
