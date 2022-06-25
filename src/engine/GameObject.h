#ifndef ALIEN3D_GAMEOBJECT_H
#define ALIEN3D_GAMEOBJECT_H
#include <vector>
#include <memory>
#include <unordered_map>
#include <engine/Component.h>
#include <gfx/Texture.h>
#include <gfx/Shader.h>
#include <gfx/WorldCamera.h>
#include <engine/Model.h>

// TODO: < Mesh >
#include <gfx/Buffer.h>
#include <engine/Transform.h>

enum Primitive {
    CUBE,
    NONE
};

class RenderQueue;
class GameObject {
    friend RenderQueue;
public:
    explicit GameObject(const std::string& name);

    GameObject(const std::string& name, Primitive primitiveType);

    GameObject(const std::string& name, std::shared_ptr<Model>& model);

    template<typename T, typename = typename std::enable_if<std::is_base_of<Component,T>::value>::type>
    void attachComponent(const std::shared_ptr<T>& component) {
        if(this->m_Components.count(typeid(T).name()) > 0) {
            ALIEN_ERROR("This component has already added to GameObject!");
        } else {
            this->m_Components[typeid(T).name()] = component;
        }
    }

    template<typename T, typename = typename std::enable_if<std::is_base_of<Component,T>::value>::type>
    void detachComponent(const std::shared_ptr<T>& component) {
        auto pos = this->m_Components.find(typeid(T).name());
        m_Components.erase(pos);
    }

private:
    void drawCall();
    void initGameObject();

    std::string m_ObjectUUID;
    std::string m_Name;

    Shader m_Shader;
    Texture m_Texture;
    std::shared_ptr<Model> m_Model;

    bool m_Renderable { false };
    std::shared_ptr<Transform> m_Transformation;

    std::unordered_map<std::string,std::shared_ptr<Component>> m_Components;
};

#endif //ALIEN3D_GAMEOBJECT_H
