#ifndef ALIEN3D_SPRITE_BATCHER
#define ALIEN3D_SPRITE_BATCHER

#include <Base.hpp>
#include "ShaderManager.h"

class SpriteBatcher {
public:
    SpriteBatcher() {
        UUID = Utils::getUUID();

    }

    bool add(std::shared_ptr<Sprite> &sprite) {
        if (m_Sprites.size() <= BatchSize) {
            m_Sprites.emplace_back(sprite, false);
            return true;
        } else {
            return false;
        }
    }

    void batch() {
        init();
    }

    bool full() {
        return m_Sprites.size() == BatchSize;
    }

    void batchAgain() {
        batchNewSprites();
    }

    void batchedDrawCall() {
        update();
    }



    static Gfx_i32 GetBatchSize() {
        if (BatchSize <= 0) {
            Gfx_i32 texUnits;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
            BatchSize = texUnits;
            return BatchSize;
        } else {
            return BatchSize;
        }
    }

    std::string UUID;
private:

    void init() {
        m_VertexDescriptor.setHasIndices(true);
        m_VertexDescriptor.vertexCountPerIndex = 2;

        ShaderManager::getInstance().addIncludeFile("common.glsl.h", RESOURCE_PATH("shaders/common.glsl.h"));
        m_Shader = ShaderManager::getInstance().addShader("default_sprite", RESOURCE_PATH("shaders/sprite_vertex.glsl"),
                                                          RESOURCE_PATH("shaders/sprite_fragment.glsl"));

        m_Shader.useProgram();
        glCheckError();

        bool alpha = false;
        for (auto &spritePair: m_Sprites) {
            spritePair.second = true;
            batchSprite(spritePair.first);
        }

        m_Shader.setIntArray("_Sprites", m_SpriteIDs);
        glCheckError();

        m_Buffer = std::make_unique<Buffer<f32>>(m_VertexDescriptor, BufferType::DYNAMIC);
        m_Buffer->setProgram(m_Shader.getProgram()).setBufferMaxCount().initBuffer();
        glCheckError();
    }

    void batchSprite(const std::shared_ptr<Sprite> &sprite) {
        bool alpha = false;

        if (sprite->m_AlphaTrans)
            alpha = true;

        auto unitID = m_BatchTexture.generateTexture(sprite->m_ImagePath.c_str(), "_Sprites",
                                                     m_Shader.getProgram(), alpha);

        for (int i = 0; i < sprite->m_VertexDesc.indices.size(); i++) {
            sprite->m_VertexDesc.indices[i] += (m_BatchIndex * m_SpriteIDPtr);
        }

        m_SpriteIDs.push_back(m_SpriteIDPtr);
        m_SpriteIDPtr += 1;

        static std::vector<i32> textureIDs{};
        textureIDs.clear();

        // since the texture id per vertex
        // it's relying on the Texture for unitID...
        static std::vector<f32> temp{};
        temp.clear();
        for (int i = 0; i < 4; i++) {
            textureIDs.push_back((float) unitID);

            // Add texture id for each vertex
            temp.insert(temp.end(), sprite->m_VertexDesc.interleavedData.begin() + 7 * i,
                        sprite->m_VertexDesc.interleavedData.begin() + 7 * (i + 1));
            temp.push_back(unitID);

        }

        sprite->m_VertexDesc.interleavedData.clear();
        sprite->m_VertexDesc.interleavedData.reserve(temp.size());
        sprite->m_VertexDesc.interleavedData.insert(sprite->m_VertexDesc.interleavedData.end(), temp.begin(),
                                                    temp.end());

        VertexDescriptor<f32>::BatchBuffer(m_VertexDescriptor, sprite->m_VertexDesc);
    }

    void batchNewSprites() {
        for (auto &spritePair: m_Sprites) {
            auto sprite = spritePair.first;
            auto isBatched = spritePair.second;

            if (!isBatched) {
                batchSprite(sprite);
                spritePair.second = true;
            }
        }
    }

    //TODO: That would be more effiecent
    void update() {
        m_Shader.useProgram();
        m_BatchTexture.enableTextureUnit();
        m_Buffer->enableVAO();

        m_Shader.setIntArray("_Sprites", m_SpriteIDs);
        auto cameraPos = AlienApplication::Camera.getCameraPos();
        m_Shader.setVector3("cameraPos", cameraPos);
        m_Shader.setBool("_Batch", true);

        if (GameObject::WireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        // We reset the buffers.
        m_VertexDescriptor.interleavedData.clear();
        m_VertexDescriptor.indices.clear();

        static std::vector<float> payload{};
        payload.clear();
        for (auto &spritePair: m_Sprites) {
            auto sprite = spritePair.first;
            auto pos = sprite->m_Transformation->getPosition();
            auto scale = sprite->m_Transformation->getScale();
            auto rot = sprite->m_Transformation->getRotation();

            auto lastPos = pos;

            pos.x = 0;
            pos.y = 0;

            // TODO: Move the all this to updateBatchTransform() on Transform
            static int w = AlienApplication::WIDTH, h = AlienApplication::HEIGHT;
            static float asp = ((float) h / (float) w);

            // normalization
            scale.x *= (asp);
            scale.y *= (asp);

            // translation on 2d
            static auto x = 0.0f, y = 0.0f;

            //--
            x = (pos.x + (-scale.x));
            y = (pos.y - scale.y);

            static auto x0 = 0.0f, y0 = 0.0f;
            x0 = x * (float) cos(rot.z) - y * (float) sin(rot.z);
            y0 = x * (float) sin(rot.z) + y * (float) cos(rot.z);

            sprite->m_VertexDesc.interleavedData[0] = x0 * asp;
            sprite->m_VertexDesc.interleavedData[1] = y0 * asp;

            //--
            x = (pos.x + (-scale.x));
            y = (pos.y - (-scale.y));

            x0 = x * (float) cos(rot.z) - y * (float) sin(rot.z);
            y0 = x * (float) sin(rot.z) + y * (float) cos(rot.z);

            sprite->m_VertexDesc.interleavedData[8] = x0 * asp;
            sprite->m_VertexDesc.interleavedData[9] = y0 * asp;

            //--
            x = (pos.x + scale.x);
            y = (pos.y - (-scale.y));

            x0 = x * (float) cos(rot.z) - y * (float) sin(rot.z);
            y0 = x * (float) sin(rot.z) + y * (float) cos(rot.z);

            sprite->m_VertexDesc.interleavedData[16] = x0 * asp;
            sprite->m_VertexDesc.interleavedData[17] = y0 * asp;

            //--
            x = (pos.x + scale.x);
            y = (pos.y - scale.y);

            x0 = x * (float) cos(rot.z) - y * (float) sin(rot.z);
            y0 = x * (float) sin(rot.z) + y * (float) cos(rot.z);

            sprite->m_VertexDesc.interleavedData[24] = x0 * asp;
            sprite->m_VertexDesc.interleavedData[25] = y0 * asp;

            //--
            sprite->m_VertexDesc.interleavedData[0] += lastPos.x;
            sprite->m_VertexDesc.interleavedData[1] += lastPos.y;
            sprite->m_VertexDesc.interleavedData[8] += lastPos.x;
            sprite->m_VertexDesc.interleavedData[9] += lastPos.y;
            sprite->m_VertexDesc.interleavedData[16] += lastPos.x;
            sprite->m_VertexDesc.interleavedData[17] += lastPos.y;
            sprite->m_VertexDesc.interleavedData[24] += lastPos.x;
            sprite->m_VertexDesc.interleavedData[25] += lastPos.y;

            pos.x = lastPos.x;
            pos.y = lastPos.y;

            // TODO: rotation on 2d
            VertexDescriptor<f32>::BatchBuffer(m_VertexDescriptor, sprite->m_VertexDesc);
        }

        // TODO: This may not be performance-wise (copying)
        m_Buffer->m_VertDesc = m_VertexDescriptor;
        glCheckError();

        glNamedBufferSubData(m_Buffer->m_VBO, 0, m_Buffer->m_VertDesc.interleavedData.size() * sizeof(f32),
                             &m_Buffer->m_VertDesc.interleavedData[0]);
        glCheckError();

        glNamedBufferSubData(m_Buffer->m_EBO, 0, m_Buffer->m_VertDesc.indices.size() * sizeof(Gfx_u32),
                             &m_Buffer->m_VertDesc.indices[0]);
        glCheckError();

        glDrawElements(GL_TRIANGLES, m_Buffer->m_VertDesc.indices.size(), GL_UNSIGNED_INT, nullptr);
        glCheckError();

        m_Shader.disableProgram();
    }

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