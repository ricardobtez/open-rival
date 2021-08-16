#ifndef TEXT_RENDERABLE_H
#define TEXT_RENDERABLE_H

#include <string>

#include <glm/glm.hpp>

#include "Font.h"
#include "Spritesheet.h"

namespace Rival {

    /**
     * Class that allows colored text to be rendered as textured quads.
     *
     * This creates the VAO and all necessary buffers.
     */
    class TextRenderable {

    public:
        static const int numVertexDimensions = 3;    // x, y, z
        static const int numTexCoordDimensions = 2;  // u, v
        static const int numColorDimensions = 3;     // r, g, b
        static const int numVerticesPerChar = 4;

        /*
         * 6 indices are required to render a quad using GL_TRIANGLES:
         *  - First triangle: 0-1-2
         *  - Second triangle: 2-3-0
         */
        static const int numIndicesPerChar = 6;

        mutable bool dirty = true;

        /**
         * Constructs a TextRenderable.
         */
        TextRenderable(
                const std::string text, const Font& font, float x, float y);

        // Disable moving / copying
        TextRenderable(const TextRenderable& other) = delete;
        TextRenderable(TextRenderable&& other) = delete;
        TextRenderable& operator=(const TextRenderable& other) = delete;
        TextRenderable& operator=(TextRenderable&& other) = delete;

        GLuint getVao() const { return vao; }
        GLuint getPositionVbo() const { return positionVbo; }
        GLuint getTexCoordVbo() const { return texCoordVbo; }
        GLuint getColorVbo() const { return colorVbo; }
        GLuint getIbo() const { return ibo; }

        GLuint getTextureId() const;

        const Font* getFont() const { return font; }
        std::string getText() const { return text; }
        float getX() const { return x; }
        float getY() const { return y; }

    private:
        GLuint vao;
        GLuint positionVbo;
        GLuint texCoordVbo;
        GLuint colorVbo;
        GLuint ibo;

        const Font* font;
        std::string text;

        float x;
        float y;
    };

}  // namespace Rival

#endif  // TEXT_RENDERABLE_H