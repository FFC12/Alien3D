#ifndef ALIEN3D_GAMEOBJECT_H
#define ALIEN3D_GAMEOBJECT_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <engine/Component.h>
#include <engine/Light.h>
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

class Light;

class SpriteBatcher;

class WorldSimulation;

class Sprite;

class GameObject {
    friend class RenderQueue;

    friend class Sprite;

    friend class Light;

    friend class WorldSimulation;

    friend class SpriteBatcher;

public:
    explicit GameObject(const std::string &name);

    GameObject(const std::string &name, Primitive primitiveType);

    GameObject(const std::string &name, std::shared_ptr<Model> &model);

//    ~GameObject() {
//        for (auto &i: m_Components) {
//            delete i.second;
//        }
//    }

    // Careful! shared_ptr does not use as what it is *ref-counting*, instead function just gets the raw pointer
    template<typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    void attachComponentRC(std::shared_ptr<T> &component, const std::string &name) {
        if (this->m_Components.count(name) > 0) {
            ALIEN_ERROR("This component has already added to GameObject!");
        } else {
            this->m_Components[name] = component.get();
        }
    }

    template<typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    void attachComponent(T *component, const std::string &name) {
        if (this->m_Components.count(name) > 0) {
            ALIEN_ERROR("This component has already added to GameObject!");
        } else {
            this->m_Components[name] = component;
        }
    }

    template<typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    void detachComponent(const std::shared_ptr<T> &component, const std::string &name) {
        auto pos = this->m_Components.find(name);
        m_Components.erase(pos);
    }

    template<typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    T *getComponent(const std::string &name) {
        if (this->m_Components.count(name) > 0) {
            auto pos = this->m_Components.find(name);
            return ((T *) pos->second);
        } else {
            return nullptr;
        }
    }

    std::string getObjectUUID() {
        return this->m_ObjectUUID;
    }

    // TODO: add getComponent function..
protected:
    virtual void drawCall();

    void initObject();

    void initGameObject();

    void components();

    static void GameobjectWidget();

    std::string m_ObjectUUID;
    std::string m_Name;

    Shader m_Shader;
    Texture m_Texture;

//    Transform m_Transformation;
    std::shared_ptr<Model> m_Model;

    bool m_Renderable{false};
    bool m_IsSprite{false};
    std::map<std::string, Component *> m_Components;

    static inline bool WidgetInitialized{false};
    static inline bool WireframeMode{false};
    static inline std::unordered_map<std::string, GameObject *> GameobjectList{};
};

#endif //ALIEN3D_GAMEOBJECT_H
