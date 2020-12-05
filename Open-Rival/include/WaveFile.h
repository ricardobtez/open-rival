#ifndef WAVE_FILE_H
#define WAVE_FILE_H

#include <cstdint>
#include <string>
#include <vector>

namespace Rival {

    typedef struct WavHeader {
        /* RIFF Chunk Descriptor */
        std::uint8_t RIFF[4];     // RIFF Header Magic header
        std::uint32_t ChunkSize;  // RIFF Chunk Size
        std::uint8_t WAVE[4];     // WAVE Header
        /* "fmt" sub-chunk */
        std::uint8_t fmt[4];          // FMT header
        std::uint32_t Subchunk1Size;  // Size of the fmt chunk
        std::uint16_t AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        std::uint16_t NumOfChan;      // Number of channels 1=Mono 2=Sterio
        std::uint32_t SamplesPerSec;  // Sampling Frequency in Hz
        std::uint32_t bytesPerSec;    // bytes per second
        std::uint16_t blockAlign;     // 2=16-bit mono, 4=16-bit stereo
        std::uint16_t bitsPerSample;  // Number of bits per sample
        /* "data" sub-chunk */
        std::uint8_t Subchunk2ID[4];  // "data"  string
        std::uint32_t Subchunk2Size;  // Sampled data length
    } WavHeader;

    class WaveFile {

    public:
        WavHeader header;
        std::vector<std::uint8_t> data;

        WaveFile(std::string filename);
    };

}  // namespace Rival

#endif  // WAVE_FILE_H
