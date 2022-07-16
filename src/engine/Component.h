#ifndef ALIEN3D_COMPONENT_H
#define ALIEN3D_COMPONENT_H

#include <utils/Uuid.h>

enum ComponentType {
    SPRITE_ANIM,
    PHYSICS_BODY,
    TRANSFORM,
    GAME_SCRIPT,
    SHADER,
    TEXTURE,
    NO_COMP
};

class Component {
public:
    Component() : componentType(NO_COMP) {
        this->m_UUID = Utils::getUUID();
    }

//    virtual ~Component() = default;

    virtual void OnComponentWidgetDrawn() = 0;

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
