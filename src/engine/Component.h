#ifndef ALIEN3D_COMPONENT_H
#define ALIEN3D_COMPONENT_H

#include <utils/Uuid.h>

class Scene;

enum ComponentType {
    SPRITE_ANIM = 0,
    PHYSICS_BODY = 1,
    TRANSFORM = 2,
    GAME_SCRIPT = 3,
    SHADER = 4,
    TEXTURE = 5,
    NO_COMP = 20
};

class Component {
public:
    Component() : componentType(NO_COMP) {
        this->m_UUID = Utils::getUUID();
    }

//    virtual ~Component() = default;

    virtual void OnComponentWidgetDrawn() = 0;
    virtual void OnSceneSerialized(Scene& scene) = 0;
    virtual void OnSceneDeserialized(Scene& scene) = 0;

    ComponentType componentType;
protected:

    std::string m_UUID;
private:
};

class GameObject;

static Component* Create(const ComponentType&,GameObject&);

class Sprite;

static Component* Create(const ComponentType&,Sprite&);

#endif //ALIEN3D_COMPONENT_H
