#ifndef ALIEN3D_TEXTURE_H
#define ALIEN3D_TEXTURE_H

#include <gfx/GfxBase.h>
#include <engine/Component.h>

// Add other options as well.
enum TextureFilteringMode {
    LINEAR = GL_LINEAR,
    NEAREST = GL_NEAREST
};

enum TextureWrappingMode {
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    CLAMP_TO_MIRRORED = GL_MIRRORED_REPEAT,
    REPEAT = GL_REPEAT,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
};

class Texture : public Component {
public:
    Texture() = default;

    Texture(const std::string &imagePath,
            TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
            TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
            TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
            TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR) {
        generateTexture(imagePath.c_str(), wrappingModeS, wrappingModeT, filteringModeMin, filteringModeMag);
    };

    Texture(const std::string &imagePath,
            Gfx_u32 shaderProgram,
            const std::string &texName,
            TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
            TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
            TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
            TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR) {
        generateTexture(imagePath.c_str(), shaderProgram, texName.c_str(), wrappingModeS, wrappingModeT,
                        filteringModeMin, filteringModeMag);
    };

    ~Texture() {
        glDeleteTextures(1, &m_TextureID);
    }

    void generateTexture(const char *path,
                         TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
                         TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
                         TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
                         TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR);

    void generateTexture(const char *path,
                         Gfx_u32 shaderProgram,
                         const char *texName,
                         TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
                         TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
                         TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
                         TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR);

    [[nodiscard]] Gfx_u32 enableTexture() const {
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

private:
    Gfx_u32 m_CurrUnitID{0};
    Gfx_u32 m_TextureID{0};
};


#endif //ALIEN3D_TEXTURE_H
