#ifndef ALIEN3D_TRANSFORM_H
#define ALIEN3D_TRANSFORM_H

#include <engine/Vector.h>
#include <engine/Component.h>
#include <editor/EditorWindow.h>

class GameObject;

class Sprite;

class Transform : public Component {
    friend class GameObject;

    friend class Sprite;

public:
    Transform() : m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f) {
//        COMPONENT_WIDGET_REGISTRY("transform", Transform::transformWidget);
    }

    Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
            : m_Position(position), m_Rotation(rotation), m_Scale(scale) {
//        COMPONENT_WIDGET_REGISTRY("transform", Transform::transformWidget);
    }

    ~Transform() {
        m_RegisteredCallbacks.clear();
    }

    void setModelMatrix(glm::mat4 &m) {
        m_ModelMatrix = m;
    }

    glm::mat4 getModelMatrix() {
        return this->m_ModelMatrix;
    }

    Vector3 getPosition() {
        return m_Position;
    }

    Vector3 getScale() {
        return m_Scale;
    }

    Vector3 getRotation() {
        return m_Rotation;
    }

    Vector3 positionLerp(Vector3 newPosition, float s) {
        static Vector3 lastPoint{m_Position.x, m_Position.y, m_Position.z};

        float x, y;

        if (abs(lastPoint.x - newPosition.x) > FLT_EPSILON && abs(lastPoint.y - newPosition.y) > FLT_EPSILON) {
            x = lastPoint.x + s * (newPosition.x - lastPoint.x);
            y = lastPoint.y + s * (newPosition.y - lastPoint.y);

            lastPoint.x = x;
            lastPoint.y = y;

            m_Position.x = x;
            m_Position.y = y;
        } else {
            x = newPosition.x;
            y = newPosition.y;

            lastPoint.x = x;
            lastPoint.y = y;

            m_Position.x = x;
            m_Position.y = y;
        }

        return {x, y, lastPoint.x};
    }

    void registerCallback(std::function<void(const Vector3&,const Vector3&, const Vector3&)> callback) {
       m_RegisteredCallbacks.push_back(callback);
    }

    void setPosition(const Vector3 &position) {
        m_Position = position;
    }

    void setPosition(float x, float y) {
        m_Position = Vector3(x, y, m_Position.z);
    }


    void setRotation(const Vector3 &rotation) {
        m_Rotation = rotation;
    }

    void setRotation(float z) {
        m_Rotation = Vector3(m_Rotation.x, m_Rotation.y, z);
    }

    void setScale(const Vector3 &scale) {
        m_Scale = scale;
    }

    void OnComponentWidgetDrawn() override {
        transformWidget();
    }

private:
    void updateTransform(bool isSprite) {
        if (!isSprite) {
            m_ModelMatrix = glm::mat4(1.0f);
            m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));
            m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(m_Position.x, m_Position.y, m_Position.z));
        } else {
            m_ModelMatrix = glm::mat4(1.0f);
            m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(m_Position.x, m_Position.y, 0.0f));
            m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale.x, m_Scale.y, 1.0f));
//            m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//            m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }
    }

    void updateBatchTransform() {

    }

    void localRotate(Vector3 &p, Vector3 &c) {

    }

    void transformWidget() {
        if (ImGui::TreeNode("Transform")) {
            f32 pos[3] = {m_Position.x, m_Position.y, m_Position.z};
            f32 rot[3] = {m_Rotation.x, m_Rotation.y, m_Rotation.z};
            f32 sca[3] = {m_Scale.x, m_Scale.y, m_Scale.z};

            if (ImGui::TreeNode("Position")) {

                ImGui::DragFloat("X", &pos[0], 0.005f);
                ImGui::DragFloat("Y", &pos[1], 0.005f);
                ImGui::DragFloat("Z", &pos[2], 0.005f);

                m_Position.x = pos[0];
                m_Position.y = pos[1];
                m_Position.z = pos[2];

                ImGui::TreePop();
            }
            ImGui::Separator();

            if (ImGui::TreeNode("Rotation")) {
                ImGui::DragFloat("X", &rot[0], 0.5f);
                ImGui::DragFloat("Y", &rot[1], 0.5f);
                ImGui::DragFloat("Z", &rot[2], 0.5f);

                m_Rotation.x = rot[0];
                m_Rotation.y = rot[1];
                m_Rotation.z = rot[2];

                ImGui::TreePop();
            }
            ImGui::Separator();

            if (ImGui::TreeNode("Scale")) {
                ImGui::DragFloat("X", &sca[0], 0.05f);
                ImGui::DragFloat("Y", &sca[1], 0.05f);
                ImGui::DragFloat("Z", &sca[2], 0.05f);

                m_Scale.x = sca[0];
                m_Scale.y = sca[1];
                m_Scale.z = sca[2];

                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        for (const auto& callback: m_RegisteredCallbacks) {
            callback(m_Position, m_Rotation, m_Scale);
        }

        ImGui::Separator();
    }

    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Scale;

    std::vector<std::function<void(Vector3 &, Vector3 &, Vector3 &)>> m_RegisteredCallbacks;

    glm::mat4 m_ModelMatrix{1.0f};
};

#endif //ALIEN3D_TRANSFORM_H
