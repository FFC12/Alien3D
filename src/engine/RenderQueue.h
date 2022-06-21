#ifndef ALIEN3D_RENDERQUEUE_H
#define ALIEN3D_RENDERQUEUE_H
#include <map>
#include <memory>
#include <Base.hpp>
#include <engine/GameObject.h>

class RenderQueue {
private:
    std::map<std::string, std::shared_ptr<GameObject>> m_RenderQueue;

    void renderGameObject(GameObject& gameObject) {
        if(gameObject.m_Renderable) {
            gameObject.drawCall();
        }
    }
public:
    RenderQueue() = default;

    void addQueue(const std::shared_ptr<GameObject>& gameObject) {
        m_RenderQueue[gameObject->m_ObjectUUID] = gameObject;
    }

    void deleteQueue(const GameObject& gameObject) {
        auto pos = m_RenderQueue.find(gameObject.m_ObjectUUID);
        m_RenderQueue.erase(pos);
    }

    void render() {
        std::map<std::string, std::shared_ptr<GameObject>>::iterator it;
        for(it = m_RenderQueue.begin(); it != m_RenderQueue.end(); ++it) {
            this->renderGameObject(*it->second);
        }
    }
};

#endif //ALIEN3D_RENDERQUEUE_H
