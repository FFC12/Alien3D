#ifndef ALIEN3D_WORLDCAMERA_H
#define ALIEN3D_WORLDCAMERA_H

#include <Base.hpp>
#include <editor/EditorWindow.h>
#include <gfx/GfxBase.h>
#include <engine/Vector.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class WorldCamera {
private:
    const glm::vec3 m_UpUnit = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 m_ViewMatrix{1.0f};
    glm::mat4 m_ProjMatrix{1.0f};

    bool m_MouseJustInitialized{true};
    bool m_IsOrtho{false};

    float m_Speed = {2.0f};
    float m_Yaw = {-90.0f};
    float m_Pitch = {0.0f};
    float m_Fov = {45.0f};

    // OpenGL Z-axis is reversed (+5 -> -5)
    glm::vec3 m_Pos{0.0f, 0.0f, 5.0f};
    glm::vec3 m_Target{0.0f, 0.0f, 0.0f};
    glm::vec3 m_Direction{0.0f, 0.0f, 0.0f};

    glm::vec3 m_CamUp{0.0f}, m_CamForward{0.0f}, m_CamRight{0.0f};

    u32 m_Width, m_Height;
    bool m_IsEditorView{false};

    ImVec4 m_BackgroundColor{};
public:
    explicit WorldCamera(bool isEditorView = true)
            : m_IsEditorView(isEditorView) {
        COMPONENT_WIDGET_REGISTRY("camera", WorldCamera::cameraWidget);
    };

    WorldCamera(glm::vec3 initPos, bool isEditorView = true) : m_Pos(initPos), m_IsEditorView(isEditorView) {}

    void setWindowSize(const u32 &w, const u32 &h) {
        m_Width = w;
        m_Height = h;
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return this->m_ViewMatrix;
    }

    [[nodiscard]] glm::mat4 getProjMatrix() const {
        return this->m_ProjMatrix;
    }

    glm::vec3 getCameraPos() const {
        return this->m_Pos;
    }

    glm::vec4 getCameraBackground() {
        return glm::vec4(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z,
                         m_BackgroundColor.w);
    }

    void updateCamera() {
        if (m_IsEditorView) {
            m_CamRight = glm::normalize(glm::cross(m_UpUnit, m_CamForward));
            m_CamUp = glm::normalize(glm::cross(m_CamForward, m_CamRight));

            if (!m_IsOrtho) {
                m_ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_CamForward, m_CamUp);
                auto aspect = (float) m_Width / m_Height;
                m_ProjMatrix = glm::perspective(glm::radians(m_Fov), aspect,
                                                0.1f, 100.0f);
            } else {
                m_ProjMatrix = glm::ortho(0.0f, (float) m_Height, 0.0f, (float) m_Width, 0.1f, 100.0f);
            }
        } else {
            // maybe we should also set the matrices here as well...
        }
    }

    void setOrthoCamera(bool v) {
        this->m_IsOrtho = v;
    }

    bool isOrtho() {
        return this->m_IsOrtho;
    }

    void setPosition(Vector3 v) {
        this->m_Pos = glm::vec3(v.x, v.y, v.z);
    }

    void cameraMoveForwardEvent(float delta) {
        if (!m_IsOrtho)
            m_Pos += m_Speed * m_CamForward * delta;
        else {
            m_Pos.y -= 0.5f * m_Speed * delta;
        }
    }

    void cameraMoveBackwardEvent(float delta) {
        if (!m_IsOrtho)
            m_Pos -= m_Speed * m_CamForward * delta;
        else {
            m_Pos.y += 0.5f * m_Speed * delta;
        }
    }

    void cameraMoveRightEvent(float delta) {
        if (!m_IsOrtho) {
            auto right = glm::cross(m_CamForward, m_CamUp);
            m_Pos += glm::normalize(right) * m_Speed * delta;
        } else {
            m_Pos.x -= 0.5f * m_Speed * delta;
        }
    }

    void cameraMoveLeftEvent(float delta) {
        if (!m_IsOrtho) {
            auto right = glm::cross(m_CamForward, m_CamUp);
            m_Pos -= glm::normalize(right) * m_Speed * delta;
        } else {
            m_Pos.x += 0.5f * m_Speed * delta;
        }
    }

    void cameraFreeLookEvent(double xPos, double yPos, double s = 0.1f) {
        static double lastX = m_Width / 2, lastY = m_Height / 2;
        if (!m_IsOrtho) {

            if (m_MouseJustInitialized) {
                lastX = xPos;
                lastY = yPos;
                m_MouseJustInitialized = false;
            }

            float offsetX = xPos - lastX;
            float offsetY = lastY - yPos;

            lastX = xPos;
            lastY = yPos;

            offsetX *= s;
            offsetY *= s;

            m_Yaw += offsetX;
            m_Pitch += offsetY;

            if (m_Pitch > 89.0f) {
                m_Pitch = 89.0f;
            }

            if (m_Pitch < -89.0f) {
                m_Pitch = -89.0f;
            }

            glm::vec3 front;
            front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            front.y = sin(glm::radians(m_Pitch));
            front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

            m_CamForward = glm::normalize(front);
        }
    }

    void cameraScrollEvent(double xOffset, double yOffset) {
        m_Fov -= (float) yOffset;

        if (m_Fov <= 0.2f) {
            m_Fov = 0.1f;
        }

        if (m_Fov >= 100.0f) {
            m_Fov = 0.0f;
        }
    }

    void cameraWidget() {
        f32 pos[3] = {m_Pos.x, m_Pos.y, m_Pos.z};

        ImGui::Text("Camera Position:");
        ImGui::BeginGroup();

        ImGui::DragFloat("X ", &pos[0], 0.005f);
        ImGui::DragFloat("Y ", &pos[1], 0.005f);
        ImGui::DragFloat("Z ", &pos[2], 0.005f);

        m_Pos.x = pos[0];
        m_Pos.y = pos[1];
        m_Pos.z = pos[2];


        ImGuiColorEditFlags miscFlags =
                ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf |
                ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions;
        ImGui::Text("Camera Background Color:");
        ImGui::ColorEdit4("Camera Background##0", (float *) &m_BackgroundColor,
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | miscFlags);

        ImGui::EndGroup();
    }


};


#endif //ALIEN3D_WORLDCAMERA_H
