#ifndef ALIEN3D_SPRITE_H
#define ALIEN3D_SPRITE_H

#include <engine/GameObject.h>
#include <Application.h>

class SpriteBatcher;

class WorldSimulation;

class SpriteAnimation;

class Sprite : public GameObject {

    friend class SpriteBatcher;

    friend class WorldSimulation;

    friend class SpriteAnimation;

public:
    explicit Sprite(const std::string &name, bool alpha) : GameObject(name), m_AlphaTrans(alpha) {
        AlienApplication::Camera.setOrthoCamera(true);
        AlienApplication::Camera.setPosition(Vector3(0.0f, 0.0f, 0.0f));

        if (!Warning) {
            ALIEN_INFO("Ortho camera is enabled since you created a sprite! *you supposed to be aware* ");
            Warning = true;
        }

        // by default image
    }

    Sprite(const std::string &name, const std::string &path, bool batch, bool alpha) : GameObject(name),
                                                                                       m_ImagePath(path),
                                                                                       m_IsBatcheable(batch),
                                                                                       m_AlphaTrans(alpha) {
        AlienApplication::Camera.setOrthoCamera(true);
        AlienApplication::Camera.setPosition(Vector3(0.0f, 0.0f, 0.0f));

        if (!Warning) {
            ALIEN_INFO("Ortho camera is enabled since you created a sprite! *you supposed to be aware* ");
            Warning = true;
        }

        initSprite(path);
    }

private:

    void initSprite(const std::string &path);

    std::string m_ImagePath;
    bool m_IsBatcheable{};
    bool m_AlphaTrans;

    VertexDescriptor<f32> m_VertexDesc;
    static inline bool Warning{false};
};


#endif //ALIEN3D_SPRITE_H
