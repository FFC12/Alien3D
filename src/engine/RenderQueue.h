#ifndef ALIEN3D_RENDERQUEUE_H
#define ALIEN3D_RENDERQUEUE_H

#include <map>
#include <memory>
#include <Base.hpp>
#include <engine/GameObject.h>
#include <engine/SpriteBatcher.h>

class RenderQueue {
private:
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_RenderQueue;
    std::unordered_map<std::string, std::shared_ptr<SpriteBatcher>> m_BatchedRenderQueue;

    void renderGameObject(GameObject &gameObject) {
        if (gameObject.m_Renderable) {
            gameObject.drawCall();
        }
    }

    void renderSpriteBatcher(SpriteBatcher &spriteBatcher) {
        spriteBatcher.batchedDrawCall();
    }

public:
    RenderQueue() = default;

    void addQueue(const std::shared_ptr<GameObject> &gameObject) {
        m_RenderQueue[gameObject->m_ObjectUUID] = gameObject;
    }

    void addQueue(const std::shared_ptr<SpriteBatcher> &spriteBatcher) {
        m_BatchedRenderQueue[spriteBatcher->UUID] = spriteBatcher;
    }

    void deleteQueue(const GameObject &gameObject) {
        auto pos = m_RenderQueue.find(gameObject.m_ObjectUUID);
        m_RenderQueue.erase(pos);
    }

    void deleteQueue(const SpriteBatcher &spriteBatcher) {
        auto pos = m_BatchedRenderQueue.find(spriteBatcher.UUID);
        m_BatchedRenderQueue.erase(pos);
    }

    void render() {
        static std::unordered_map<std::string, std::shared_ptr<GameObject>>::iterator gameObjectIter;
        for (gameObjectIter = m_RenderQueue.begin(); gameObjectIter != m_RenderQueue.end(); ++gameObjectIter) {
            this->renderGameObject(*gameObjectIter->second);
        }

        static std::unordered_map<std::string, std::shared_ptr<SpriteBatcher>>::iterator batcherIter;
        for (batcherIter = m_BatchedRenderQueue.begin(); batcherIter != m_BatchedRenderQueue.end(); ++batcherIter) {
            this->renderSpriteBatcher(*batcherIter->second);
        }
    }
};

#endif //ALIEN3D_RENDERQUEUE_H
