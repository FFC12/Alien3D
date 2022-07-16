#ifndef ALIEN3D_TEXTURE_H
#define ALIEN3D_TEXTURE_H

#include <gfx/GfxBase.h>
#include <engine/Component.h>
#include <unordered_map>
#include <optional>
#include <map>
#include <memory>
#include "Image.h"
#include "imgui.h"

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

class SpriteAnimation;

class Texture : public Component {
    friend class SpriteAnimation;

    friend class SpriteBatcher;

public:
    Texture() {
        componentType = TEXTURE;
    }

    Texture(const std::string &imagePath,
            Gfx_u32 shaderProgram,
            const std::string &texName,
            TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
            TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
            TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
            TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR) {
        componentType = TEXTURE;
        generateTexture(imagePath.c_str(), shaderProgram, texName.c_str(), wrappingModeS, wrappingModeT,
                        filteringModeMin, filteringModeMag);
    };

    ~Texture();

    void generateTexture(const char *path,
                         Gfx_u32 shaderProgram,
                         const char *texName,
                         TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
                         TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
                         TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
                         TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR);

    Gfx_u32 generateTexture(const char *path,
                            const char *texName,
                            Gfx_u32 shaderProgram,
                            bool isAlphaTransparent = true,
                            TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
                            TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
                            TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
                            TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR);

    Gfx_u32 enableTexture() {
        for (auto texture: m_TextureIDs) {
            if (texture.second.has_value()) {
//                glBindTextureUnit(texture.second.value(), texture.first);
                glActiveTexture(GL_TEXTURE0 + texture.second.value());
                glCheckError();
                glBindTexture(GL_TEXTURE_2D, texture.first);
                glCheckError();
            }
        }
    }

    void enableTextureUnit() {
        for (auto texture: m_TextureIDs) {
            if (texture.second.has_value()) {
                glBindTextureUnit(texture.second.value(), texture.first);
                glCheckError();
//                glActiveTexture(GL_TEXTURE0 + texture.second.value());
//                glBindTexture(GL_TEXTURE_2D, texture.first);
            }
        }
    }

    void OnComponentWidgetDrawn() override {
        textureWidget();
    }

private:
    void textureWidget() {
        if (ImGui::TreeNode("Texture")) {
            if (m_ImagePath.empty()) {
                for (const auto &imagePath: m_BatchImagePaths) {
                    auto image = TextureDataCache[imagePath];
                    auto unit = TextureCaches[imagePath];
                    auto size = image->getImageSize();
                    auto w = size.first;
                    auto h = size.second;
                    ImGui::BulletText("Path: %s", imagePath.c_str());
//                ImGui::BulletText("Unit ID: %d", unit.second);
                    ImGui::BulletText("Image Width: %d", w);
                    ImGui::BulletText("Image Height: %d", h);
                    ImGui::Separator();
                }
            } else {
                auto image = TextureDataCache[m_ImagePath];
                auto unit = TextureCaches[m_ImagePath];
                auto size = image->getImageSize();
                auto w = size.first;
                auto h = size.second;
                ImGui::BulletText("Path: %s", m_ImagePath.c_str());
//                ImGui::BulletText("Unit ID: %d", unit.second);
                ImGui::BulletText("Image Width: %d", w);
                ImGui::BulletText("Image Height: %d", h);
                ImGui::Separator();
            }
            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    Gfx_u32 m_CurrUnitID{0};

    std::string m_ImagePath;

    std::vector<std::string> m_BatchImagePaths;

    // Texture ID - Unit ID
    std::map<Gfx_u32, std::optional<Gfx_u32>> m_TextureIDs{};

    // <path, pair < TexID, UnitID > >
    static inline std::unordered_map<std::string, std::pair<Gfx_u32, Gfx_u32>> TextureCaches{};

    static inline std::unordered_map<std::string, std::shared_ptr<Image>> TextureDataCache{};
};

#endif //ALIEN3D_TEXTURE_H
