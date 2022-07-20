#ifndef ALIEN3D_SPRITE_H
#define ALIEN3D_SPRITE_H

#include <engine/GameObject.h>

class WorldSimulation;

class SpriteAnimation;

class EditorWindow;

class Sprite : public GameObject {

    friend class EditorWindow;

    friend class WorldSimulation;

    friend class SpriteAnimation;

public:
    explicit Sprite(const std::string &name);

    Sprite(const std::string &name, bool alpha);

    Sprite(const std::string &name, const std::string &path, bool alpha);

    Sprite(const Sprite &sprite);

    ~Sprite();

    static std::unique_ptr<Sprite> Create(const std::string &name, const std::string &path, bool alpha) {
        return std::move(std::make_unique<Sprite>(name, path, alpha));
    }

    void duplicate(const std::string &n, Sprite &o); 

    static inline std::unordered_map<std::string, Sprite *> SpriteList;
private:
    void initSprite(const std::string &name, const std::string &path);

    static void QuickCreate(const std::string &name) {
        auto sprite = new Sprite(name);
        sprite->initSprite(name, RESOURCE_PATH("default.jpg"));
    }

    std::string m_ImagePath;
    bool m_AlphaTrans;

    VertexDescriptor<f32> m_VertexDesc;
    static inline bool Warning{false};
};


#endif //ALIEN3D_SPRITE_H
