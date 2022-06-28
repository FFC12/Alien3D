#ifndef ALIEN3D_TEXTURE_H
#define ALIEN3D_TEXTURE_H

#include <gfx/GfxBase.h>
#include <engine/Component.h>
#include <unordered_map>
#include <optional>

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
        for (auto texture: m_TextureIDs) {
            glDeleteTextures(1, &texture.first);
        }
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

    Gfx_u32 enableTexture() {
        for (auto texture: m_TextureIDs) {
            if (texture.second.has_value()) {
                glActiveTexture(GL_TEXTURE0 + texture.second.value());
                glBindTexture(GL_TEXTURE_2D, texture.first);
            }
        }
    }

private:
    Gfx_u32 m_CurrUnitID{0};

    // Texture ID - Unit ID
    std::unordered_map<Gfx_u32, std::optional<Gfx_u32>> m_TextureIDs{};

    // <path, pair < TexID, UnitID > >
    static inline std::unordered_map<std::string, std::pair<Gfx_u32, Gfx_u32>> TextureCaches{};
};


#endif //ALIEN3D_TEXTURE_H
