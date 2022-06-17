#include "GameObject.h"

template<typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
void GameObject::attachComponent(T component) {
    this->m_Components.push_back(component);
}

void GameObject::detachComponent(Component component) {
}
