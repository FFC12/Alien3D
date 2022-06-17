#ifndef ALIEN3D_WORLDCAMERA_H
#define ALIEN3D_WORLDCAMERA_H

#include <Base.hpp>
#include <gfx/GfxBase.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class WorldCamera {
private:
    const glm::vec3 m_UpUnit = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 m_ViewMatrix{1.0f};
    glm::mat4 m_ProjMatrix{1.0f};

    bool m_MouseJustInitialized{true};
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
public:
    WorldCamera() = default;

    WorldCamera(glm::vec3 initPos) : m_Pos(initPos) {}

    void setWindowSize(const u32& w, const u32& h) {
        m_Width = w;
        m_Height = h;
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return this->m_ViewMatrix;
    }

    [[nodiscard]] glm::mat4 getProjMatrix() const {
        return this->m_ProjMatrix;
    }

    void updateCamera() {
        m_Direction = glm::normalize(m_Pos - m_Target);
        m_CamRight = glm::normalize(glm::cross(m_UpUnit, m_Direction));
        m_CamUp = glm::cross(m_Direction, m_CamRight);
//        m_CamForward = glm::vec3(0.0f,0.0f,-1.0f);

        m_ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_CamForward, m_CamUp);
        auto aspect = (float) m_Width / m_Height;
        m_ProjMatrix = glm::perspective(glm::radians(m_Fov), aspect,
                                      0.1f, 100.0f);
    }

    void cameraMoveForwardEvent(float delta) {
        m_Pos += m_Speed * m_CamForward * delta;
    }

    void cameraMoveBackwardEvent(float delta) {
        m_Pos -= m_Speed * m_CamForward * delta;
    }

    void cameraMoveRightEvent(float delta) {
        auto right = glm::cross(m_CamForward, m_CamUp);
        m_Pos += glm::normalize(right) * m_Speed * delta;
    }

    void cameraMoveLeftEvent(float delta) {
        auto right = glm::cross(m_CamForward, m_CamUp);
        m_Pos -= glm::normalize(right) * m_Speed * delta;
    }

    void cameraFreeLookEvent(double xPos, double yPos, double s = 0.1f) {
        static double lastX = m_Width / 2 , lastY = m_Height / 2;

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

    void cameraScrollEvent(double xOffset, double yOffset) {
        m_Fov -= (float) yOffset;

        if (m_Fov <= 0.2f) {
            m_Fov = 0.1f;
        }

        if (m_Fov >= 100.0f) {
            m_Fov = 0.0f;
        }
    }
};


#endif //ALIEN3D_WORLDCAMERA_H
