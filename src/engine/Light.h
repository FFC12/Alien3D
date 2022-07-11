#ifndef ALIEN3D_LIGHT_H
#define ALIEN3D_LIGHT_H

#include <vector>
#include <Base.hpp>
#include <engine/Vector.h>
#include <editor/EditorWindow.h>
#include <utils/Uuid.h>

enum LightType {
    PointLight,
    SunLight,
    SpotLight
};

// TODO: Multiple light feature will be added...
class Light {
public:
    Light(const Vector3 &lightPos, const Vector3 &lightColor, LightType lightType, float lightAlpha)
            : m_LightPos(lightPos),
              m_LightColor(lightColor),
              m_LightColorAlpha(lightAlpha),
              m_LightType(lightType) {
        Lights.push_back(this);
        m_ObjectUUID = Utils::getUUID();
        if (!WidgetInitialized) {
            auto objectName = "light:" + m_ObjectUUID;
            COMPONENT_WIDGET_REGISTRY(objectName, Light::LightWidget);
            WidgetInitialized = true;
        }
    }

    Vector3 getLightPos() {
        return this->m_LightPos;
    }

    Vector3 &getLightColor() {
        return m_LightColor;
    }

    LightType getLightType() const {
        return m_LightType;
    }

    static inline std::vector<Light*> Lights{};
private:
    void LightWidget() {
        static f32 pos[3] = {m_LightPos.x, m_LightPos.y, m_LightPos.z};

        ImGui::Text("Light Position:");
        ImGui::BeginGroup();

        ImGui::DragFloat("Light X ", &pos[0], 0.005f);
        ImGui::DragFloat("Light Y ", &pos[1], 0.005f);
        ImGui::DragFloat("Light Z ", &pos[2], 0.005f);

//        ALIEN_INFO(pos[0] << " " << pos[1] << " " << pos[2]);

        m_LightPos.x = pos[0];
        m_LightPos.y = pos[1];
        m_LightPos.z = pos[2];

        ImGui::EndGroup();
    }

    Vector3 m_LightPos;
    Vector3 m_LightColor;
    LightType m_LightType;

    float m_LightColorAlpha;
    std::string m_ObjectUUID;

    static inline bool WidgetInitialized{false};
};

#endif