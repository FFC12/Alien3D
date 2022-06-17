#ifndef ALIEN3D_GAMEOBJECT_H
#define ALIEN3D_GAMEOBJECT_H
#include <vector>
#include <engine/Component.h>

class GameObject {
public:
    GameObject() = default;

    template<typename T, typename>
    void attachComponent(T component);

    void detachComponent(Component component);

private:
    std::vector<Component> m_Components;
};

#endif //ALIEN3D_GAMEOBJECT_H
