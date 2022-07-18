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

class EditorWindow;

class WorldSimulation;

class Sprite;

class GameObject {
    friend class RenderQueue;

    friend class Sprite;

    friend class Light;

    friend class WorldSimulation;

    friend class EditorWindow;

public:
    explicit GameObject(const std::string &name);

//    GameObject(const GameObject& other);

    GameObject(const std::string &name, Primitive primitiveType);

    GameObject(const std::string &name, std::shared_ptr<Model> &model);

    // GFX deallocations might be performed as well. (Delete all the Texture, Shader and etc.)
    ~GameObject();

    // Careful! shared_ptr does not use as what it is *ref-counting*, instead function just gets the raw pointer
    template<typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    void attachComponentRC(std::shared_ptr<T> &component, const std::string &name, bool v) {
        if (this->m_Components.count(name) > 0) {
            ALIEN_ERROR("This component has already added to GameObject!");
        } else {
            this->m_Components[name] = {component.get(), v};
        }
    }

    void attachComponent(const std::string &type);

    void attachComponent(const ComponentType &type);

    template<typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    void attachComponent(T *component, const std::string &name, bool v) {
        if (this->m_Components.count(name) > 0) {
            ALIEN_ERROR("This component has already added to GameObject!");
        } else {
            this->m_Components[name] = {component, v};
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
            return ((T *) pos->second.first);
        } else {
            return nullptr;
        }
    }

    std::string getObjectUUID() {
        return this->m_ObjectUUID;
    }

    void duplicate(const std::string &n, GameObject &o);

    static std::unique_ptr<GameObject> Create(const std::string &name);


    // TODO: add getComponent function..
protected:
    virtual void drawCall();

    void initObject();

    void initGameObject();

    void components();

    static void GameobjectWidget();

    static void QuickCreate(const std::string &name);

    std::string m_ObjectUUID;
    std::string m_Name;

    Shader m_Shader;
    Texture *m_Texture;

//    Transform m_Transformation;
    std::shared_ptr<Model> m_Model;

    bool m_Renderable{false};
    bool m_IsSprite{false};
    Sprite *m_Sprite;
    std::map<std::string, std::pair<Component *, bool>> m_Components;
    std::map<std::string, std::pair<Component *, bool>> m_GameScripts;

    static inline bool WidgetInitialized{false};
    static inline bool WireframeMode{false};
    static inline std::unordered_map<std::string, GameObject *> GameobjectList{};
};

#endif //ALIEN3D_GAMEOBJECT_H
