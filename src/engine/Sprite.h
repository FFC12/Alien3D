#ifndef ALIEN3D_SPRITE_H
#define ALIEN3D_SPRITE_H

#include <engine/GameObject.h>

class SpriteBatcher;

class WorldSimulation;

class SpriteAnimation;

class Sprite : public GameObject {

    friend class SpriteBatcher;

    friend class WorldSimulation;

    friend class SpriteAnimation;

public:
    explicit Sprite(const std::string &name, bool alpha);

    Sprite(const std::string &name, const std::string &path, bool alpha);

private:

    void initSprite(const std::string &path);

    std::string m_ImagePath;
    bool m_AlphaTrans;

    VertexDescriptor<f32> m_VertexDesc;
    static inline bool Warning{false};
};


#endif //ALIEN3D_SPRITE_H
