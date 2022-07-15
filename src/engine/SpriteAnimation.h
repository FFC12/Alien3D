#ifndef ALIEN3D_SPRITEANIMATION_H
#define ALIEN3D_SPRITEANIMATION_H

#include <memory>
#include <Base.hpp>
#include <engine/Sprite.h>
#include <engine/Component.h>
#include <chrono>

class GameObject;

// SpriteAnimation can't use with a SpriteBatcher
class SpriteAnimation : public Component {
    friend class GameObject;

    using Time = std::chrono::duration<float, std::milli>;
    using Clock = std::chrono::high_resolution_clock;
public:
    explicit SpriteAnimation(Sprite &sprite, u32 x = 32, u32 y = 32) : m_X(x), m_Y(y), m_Sprite(sprite) {
        m_AnimID = ID++;

        auto image = Texture::TextureDataCache[m_Sprite.m_ImagePath];
        auto imageSize = image->getImageSize();

        m_W = imageSize.first;
        m_H = imageSize.second;

        m_CountX = m_W / m_X;
        m_CountY = m_H / m_Y;
    }

    void setPlaybackSpeed(f32 x) {
        m_PlaybackSpeed = x;
    }


    void setPlay(bool s) {
        m_Play = s;
    }

    void OnComponentWidgetDrawn() override {
        spriteAnimationWidget();
    }

private:

    void animate() {
        if (m_Play) {
            Time diff = Clock::now() - m_Start;
            if (m_PlaybackSpeed <= FLT_EPSILON)
                m_PlaybackSpeed = 0.000001f;

            if ((double) diff.count() >= 1.0f / m_PlaybackSpeed * 100) {
                if (m_FrameX >= m_CountX) {
                    if (m_FrameY < m_CountY) {
                        m_FrameY += 1;
                    } else {
                        m_FrameY = 1;
                    }
                    m_FrameX = 1;
                } else {
                    m_FrameX += 1;
                }

                m_Sprite.m_Shader.setFloat("_Height", (float) m_H);
                m_Sprite.m_Shader.setFloat("_Width", (float) m_W);
                m_Sprite.m_Shader.setBool("_Animation", true);
                m_Sprite.m_Shader.setFloat("_FrameSizeX", m_X);
                m_Sprite.m_Shader.setFloat("_FrameSizeY", m_Y);
                m_Sprite.m_Shader.setVector2("_FrameI", glm::vec2((f32) m_FrameX, (f32) m_FrameY));
                m_Start = Clock::now();
            }
        }
    }

    void spriteAnimationWidget() {
        if (ImGui::TreeNode("Animation")) {
            ImGui::Checkbox("Play", &m_Play);
//            ImGui::PushID(m_AnimID);
            ImGui::DragFloat("Playback Speed: ", &m_PlaybackSpeed, 0.005f, 0.0f, 10.0f);
            ImGui::BulletText("Total Frame: %d", m_CountX * m_CountY);
            ImGui::DragFloat("Frame Width:", &m_X, 0.01);
            ImGui::DragFloat("Frame Height:", &m_Y, 0.01);
//            ImGui::PopID();
            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    i32 m_AnimID;

    Clock::time_point m_Start;

    Sprite &m_Sprite;
    float m_X, m_Y;
    u32 m_W, m_H;
    u32 m_FrameX{0}, m_FrameY{1};
    u32 m_CountX, m_CountY;

    f32 m_PlaybackSpeed{1.0f};
    bool m_Play{true};

    static inline i32 ID{0};
    std::chrono::high_resolution_clock m_Clock;
};

#endif //ALIEN3D_SPRITEANIMATION_H
