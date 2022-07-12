#ifndef ALIEN3D_SPRITE_BATCHER
#define ALIEN3D_SPRITE_BATCHER

#include <Base.hpp>
#include "ShaderManager.h"
#include <engine/Sprite.h>

//TODO: Components*
// need to change the way of handling compoenents (actually no components handler rn)
class SpriteBatcher {
public:
    SpriteBatcher();

    bool add(std::shared_ptr<Sprite> &sprite);

    void batch();

    bool full();

    void batchAgain();

    void batchedDrawCall();

    static Gfx_i32 GetBatchSize();

    std::string UUID;
private:

    void init();

    void batchSprite(const std::shared_ptr<Sprite> &sprite);

    void batchNewSprites();

    //TODO: That would be more effiecent
    void update();

    std::vector<std::pair<std::shared_ptr<Sprite>, bool>> m_Sprites;

    // Sprite IDs basically for uniforms (_Sprites[n])
    std::vector<i32> m_SpriteIDs;

    i32 m_SpriteIDPtr{0};

    // This texture will be shared to each of sprites
    VertexDescriptor<f32> m_VertexDescriptor;
    Texture m_BatchTexture;
    Shader m_Shader;
    std::unique_ptr<Buffer<f32>> m_Buffer;

    static inline i32 BatchSize{};
    const u32 m_BatchIndex{4};
};

#endif