#ifndef ALIEN3D_TRANSFORM_H
#define ALIEN3D_TRANSFORM_H

#include <engine/Vector.h>
#include <engine/Component.h>
#include <editor/EditorWindow.h>

class Transform : public Component {
public:
    Transform() : m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f) {
        COMPONENT_WIDGET_REGISTRY("transform", Transform::transformWidget);
    }

    Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
            : m_Position(position), m_Rotation(rotation), m_Scale(scale) {
        COMPONENT_WIDGET_REGISTRY("transform", Transform::transformWidget);
    }

    void setModelMatrix(glm::mat4 &m) {
        m_ModelMatrix = m;
    }

    glm::mat4 getModelMatrix() {
        return this->m_ModelMatrix;
    }

    void setPosition(const Vector3 &position) {
        m_Position = position;
    }

    void setRotation(const Vector3 &rotation) {
        m_Rotation = rotation;
    }

    void setScale(const Vector3 &scale) {
        m_Scale = scale;
    }

    void updateTransform() {
        m_ModelMatrix = glm::mat4(1.0f);
        m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(m_Position.x, m_Position.y, m_Position.z));
    }

private:
    void transformWidget() {
        if (ImGui::TreeNode("Transformation")) {
            static f32 pos[3] = {m_Position.x, m_Position.y, m_Position.z};
            static f32 rot[3] = {m_Rotation.x, m_Rotation.y, m_Rotation.z};
            static f32 sca[3] = {m_Scale.x, m_Scale.y, m_Scale.z};

            ImGui::Text("Position:");
            ImGui::BeginGroup();

            ImGui::DragFloat("X ", &pos[0], 0.005f);
            ImGui::DragFloat("Y ", &pos[1], 0.005f);
            ImGui::DragFloat("Z ", &pos[2], 0.005f);

            m_Position.x = pos[0];
            m_Position.y = pos[1];
            m_Position.z = pos[2];

            ImGui::EndGroup();
            ImGui::Separator();

            ImGui::Text("Rotation:");
            ImGui::BeginGroup();

            ImGui::DragFloat("X", &rot[0], 0.5f);
            ImGui::DragFloat("Y", &rot[1], 0.5f);
            ImGui::DragFloat("Z", &rot[2], 0.5f);

            m_Rotation.x = rot[0];
            m_Rotation.y = rot[1];
            m_Rotation.z = rot[2];

            ImGui::EndGroup();

            ImGui::Text("Scale:");
            ImGui::DragFloat("X  ", &sca[0], 0.5f);
            ImGui::DragFloat("Y  ", &sca[1], 0.5f);
            ImGui::DragFloat("Z  ", &sca[2], 0.5f);

            m_Scale.x = sca[0];
            m_Scale.y = sca[1];
            m_Scale.z = sca[2];

            ImGui::Separator();

            ImGui::BeginGroup();

            ImGui::EndGroup();
            ImGui::TreePop();
        }
    }

    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Scale;

    glm::mat4 m_ModelMatrix{1.0f};
};

#endif //ALIEN3D_TRANSFORM_H
