#include "pch.h"

#include "MidiPlayer.h"

#include "SDLWrapper.h"
#include <RtMidi.h>

#include <chrono>
#include <stdexcept>

#include "TimeUtils.h"

namespace Rival {

void MidiPlayer::init()
{
    if (midiOut.isPortOpen())
    {
        return;
    }

    unsigned int nPorts = midiOut.getPortCount();
    if (nPorts == 0)
    {
        throw std::runtime_error("No MIDI output ports available!");
    }

    std::cout << "\nOpening MIDI port: " << midiOut.getPortName() << "\n";

    midiOut.openPort(0);
}

/**
 * Gets the size of a MIDI event.
 *
 * Based on:
 * https://www.midi.org/specifications-old/item/table-2-expanded-messages-list-status-bytes
 */
static int getMessageSize(uint8_t eventId)
{
    if ((eventId >= 0xC0 && eventId <= 0xDF) || eventId == 0xF3)
    {
        return 2;
    }
    else if (eventId >= 0xF4)
    {
        return 1;
    }
    return 3;
}

void MidiPlayer::play(MidiFile file)
{
    const std::scoped_lock<std::mutex> lock(playingMutex);

    if (!midiOut.isPortOpen())
    {
        throw std::runtime_error("MIDI system not initialized");
    }

    stopped = false;

    const std::vector<midi_stream_event>& events = file.getEvents();

    // Create a high-precision timer
    TimeUtils::PrecisionTimer timer;

    // Play the file until completion
    Uint32 startTime = SDL_GetTicks();
    for (auto const& evt : events)
    {
        if (stopped)
        {
            break;
        }

        std::vector<unsigned char> message;

        // Put each byte of the event into the message.
        // Events can be 1, 2 or 3 bytes long.
        std::uint32_t eventValue = evt.m_event;
        std::uint8_t eventId = eventValue & 0xFF;
        int numBytes = getMessageSize(eventId);
        message.push_back(eventId);
        if (numBytes == 2)
        {
            message.push_back((eventValue >> 8) & 0xFF);
        }
        else if (numBytes == 3)
        {
            message.push_back((eventValue >> 8) & 0xFF);
            message.push_back((eventValue >> 16) & 0xFF);
        }

        // Calculate how long we need to wait
        Uint32 currentTime = SDL_GetTicks();
        Uint32 timeElapsed = currentTime - startTime;
        int timeUntilMessage = static_cast<int>(evt.m_timestamp - timeElapsed);

        // Wait until the message is due
        if (timeUntilMessage > 0)
        {
            timer.wait(static_cast<Uint32>(timeUntilMessage));
        }

        // Send the message to our MIDI output
        midiOut.sendMessage(&message);
    }
}

void MidiPlayer::stop()
{
    stopped = true;

    // Wait until the play() method terminates
    const std::scoped_lock<std::mutex> lock(playingMutex);
}

}  // namespace Rival
