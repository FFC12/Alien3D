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
#include <utils/FileDialogs.h>
#include <filesystem>
#include <engine/SceneManager.h>

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
        generateTexture(imagePath.c_str(), shaderProgram, texName.c_str(), false, wrappingModeS, wrappingModeT,
                        filteringModeMin, filteringModeMag);
    };

    ~Texture();

    void generateTexture(const char *path,
                         Gfx_u32 shaderProgram,
                         const char *texName,
                         bool shaderProgramNeeded = true,
                         TextureWrappingMode wrappingModeS = TextureWrappingMode::REPEAT,
                         TextureWrappingMode wrappingModeT = TextureWrappingMode::REPEAT,
                         TextureFilteringMode filteringModeMin = TextureFilteringMode::LINEAR,
                         TextureFilteringMode filteringModeMag = TextureFilteringMode::LINEAR);

    void enableTexture() {
        glActiveTexture(GL_TEXTURE0 + m_UnitID);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glCheckError();
    }

    void enableTextureUnit() {
        glBindTextureUnit(m_UnitID, m_TextureID);
        glCheckError();

    }

    void OnComponentWidgetDrawn() override {
        textureWidget();

    }

    void OnSceneSerialized(Scene& scene) override {
      auto pos = this->m_ImagePath.find("res\\"); 
      auto relativeImagePath = this->m_ImagePath.substr(pos, this->m_ImagePath.npos);

      scene.writeComponent("texture", {
        {"imagePath", relativeImagePath},
        {"uuid", this->m_UUID}
      });
    }

    void OnSceneDeserialized(Scene& scene) override {

    }

    std::string getImagePath() {
        return m_ImagePath;
    }

    std::string getImageRelativePath() {
      auto pos = this->m_ImagePath.find("res\\");
      auto relativeImagePath = this->m_ImagePath.substr(pos, this->m_ImagePath.npos);

      return relativeImagePath;
    }

    void setImagePath(const std::string& path) {
      this->m_ImagePath = path;
    }

private:

    void textureWidget() {
        if (ImGui::TreeNode("Texture")) {
            auto image = TextureDataCache[m_ImagePath];
            auto size = image->getImageSize();
            auto w = size.first;
            auto h = size.second;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.6f, 0.2f, 1.0f});
            if (ImGui::ImageButton((void *) ((intptr_t) m_TextureID), ImVec2(w, h))) {
//                    ImGui::OpenPopup("Create GameObject");
                pfd::open_file file("Select Image", "", {"All Files", "*.jpg *.jpeg *.png"});
                auto selected = file.result();
                if (!selected.empty()) {
                    auto filePath = selected[0];
                    if (std::filesystem::exists(filePath)) {
                        if (m_Initialized) {
                            m_ImagePath = filePath;
                            glDeleteTextures(1, &m_TextureID);
                            m_CurrUnitID = 0;
                            generateTexture(filePath.c_str(), m_AssocProgram, "_Diffuse");
                        }
                    }
                }
            }
            ImGui::PopStyleColor();

            ImGui::BulletText("Path: %s", m_ImagePath.c_str());
//                ImGui::BulletText("Unit ID: %d", unit.second);
            ImGui::BulletText("Image Width: %d", w);
            ImGui::BulletText("Image Height: %d", h);

            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    bool m_Initialized{false};

    Gfx_u32 m_AssocProgram;

    Gfx_u32 m_CurrUnitID{0};

    std::string m_ImagePath;

    Gfx_u32 m_TextureID;
    Gfx_u32 m_UnitID;
// Texture ID - Unit ID
//    std::map<Gfx_u32, std::optional<Gfx_u32>> m_TextureIDs{};

// <path, pair < TexID, UnitID > >
//    static inline std::unordered_map<std::string, std::pair<Gfx_u32, Gfx_u32>> TextureCaches{};

    static inline std::unordered_map<std::string, std::shared_ptr<Image>> TextureDataCache{};
};

#endif //ALIEN3D_TEXTURE_H
