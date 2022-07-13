#ifndef ALIEN3D_RENDERQUEUE_H
#define ALIEN3D_RENDERQUEUE_H

#include <map>
#include <memory>
#include <Base.hpp>
#include <engine/GameObject.h>

class RenderQueue {
private:
    std::unordered_map<std::string, GameObject *> m_RenderQueue;

    void renderGameObject(GameObject &gameObject) {
        if (gameObject.m_Renderable) {
            gameObject.drawCall();
        }
    }

    RenderQueue() = default;

public:
    static RenderQueue &getInstance() {
        static RenderQueue ins;
        return ins;
    }

    void addQueue(GameObject *gameObject) {
        m_RenderQueue[gameObject->m_ObjectUUID] = gameObject;
    }

    void deleteQueue(const GameObject &gameObject) {
        auto pos = m_RenderQueue.find(gameObject.m_ObjectUUID);
        m_RenderQueue.erase(pos);
    }

    void render() {
        static std::unordered_map<std::string, GameObject *>::iterator gameObjectIter;
        for (gameObjectIter = m_RenderQueue.begin(); gameObjectIter != m_RenderQueue.end(); ++gameObjectIter) {
            if (gameObjectIter->second == nullptr)
                abort();
            this->renderGameObject(*gameObjectIter->second);
        }
    }
};

#endif //ALIEN3D_RENDERQUEUE_H
