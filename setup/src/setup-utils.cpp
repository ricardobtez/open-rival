#include "pch.h"
#include "setup-utils.h"

#include <fstream>
#include <stdio.h>
#include <vector>
#include <windows.h>

#include "Palette.h"

namespace Rival {
namespace Setup {

    /**
     * Attempts to create the given directory.
     */
    bool createDirectory(const std::string dirName) {
        return CreateDirectoryA(dirName.c_str(), NULL)
                || ERROR_ALREADY_EXISTS == GetLastError();
    }

    /**
     * Rounds a number up to the nearest power of 2.
     *
     * See:
     * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
     */
    int nextPowerOf2(int v) {

        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;

        return v;
    }

    /**
     * Writes an image to disk.
     */
    void writeImage(
            Image& image,
            const Palette::Palette& palette,
            const std::string filename) {

        // Open file for writing
        std::ofstream out;
        out.open(filename, std::ios::out | std::ios::binary);

        /*
         * Write using TGA format:
         * http://tfc.duke.free.fr/coding/tga_specs.pdf
         */

        out.put(0x00);  // ID Length
        out.put(0x01);  // Color map type
        out.put(0x01);  // Image type (uncompressed, color-mapped)

        // Color map specification
        out.put(0);  // Index of first entry
        out.put(0x00);
        out.put(0x00);  // Number of entries (256)
        out.put(0x01);
        out.put(32);  // Entry size (32-bit RGBA)

        // Image specification
        out.put(0x00);  // X-origin
        out.put(0x00);
        out.put(0x00);  // Y-origin
        out.put(0x00);
        int w = image.getWidth();
        int h = image.getHeight();
        out.put((uint8_t) w);  // Width
        out.put((uint8_t)(w >> 8));
        out.put((uint8_t) h);  // Height
        out.put((uint8_t)(h >> 8));
        out.put(8);  // Bits per pixel

        // Image descriptor byte
        // (8 = number of alpha bits, bit5: upper-left origin)
        out.put(8 | 1 << 5);

        // Color map data
        for (int i = 0; i < Palette::paletteSize; ++i) {

            const std::uint32_t col = palette[i];
            const std::uint8_t red = (uint8_t)((col & 0xFF000000) >> 24);
            const std::uint8_t green = (uint8_t)((col & 0x00FF0000) >> 16);
            const std::uint8_t blue = (uint8_t)((col & 0x0000FF00) >> 8);
            const std::uint8_t alpha = (uint8_t)(col & 0x000000FF);

            out.put(blue);
            out.put(green);
            out.put(red);
            out.put(alpha);
        }

        // Pixel data
        std::vector<std::uint8_t>* data = image.getData();
        int stride = image.getStride();
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                out.put((*data)[x + y * stride]);
            }
        }
    }

}}  // namespace Rival::Setup
