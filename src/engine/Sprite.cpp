#include "Sprite.h"
#include <Application.h>
#include "ShaderManager.h"

Sprite::Sprite(const std::string &name, bool alpha) : GameObject(name), m_AlphaTrans(alpha) {
    AlienApplication::Camera.setOrthoCamera(true);
    AlienApplication::Camera.setPosition(Vector3(0.0f, 0.0f, 0.0f));

    if (!Warning) {
        ALIEN_INFO("Ortho camera is enabled since you created a sprite! *you supposed to be aware* ");
        Warning = true;
    }

    // by default image
}

Sprite::Sprite(const std::string &name, const std::string &path, bool batch, bool alpha) : GameObject(name),
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

void Sprite::initSprite(const std::string &path) {
    this->m_Renderable = true;
    this->m_IsSprite = true;

    if (!m_IsBatcheable) {
        ShaderManager::getInstance().addIncludeFile("common.glsl.h", RESOURCE_PATH("shaders/common.glsl.h"));
        this->m_Shader = ShaderManager::getInstance().addShader("sprite_default",
                                                                RESOURCE_PATH("shaders/sprite_vertex.glsl"),
                                                                RESOURCE_PATH("shaders/sprite_fragment.glsl"));

        this->m_Shader.useProgram();
        glCheckError();

        static std::array<float, 8> vertices = {
                -1.0f, 1.0f,
                -1.0f, -1.0f,
                1.0f, -1.0f,
                1.0f, 1.0f,
        };

        static std::array<float, 12> normals = {
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f
        };

        static std::array<float, 8> texCoords = {
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
        };

        static std::array<Gfx_u32, 6> indices = {
//            3, 2, 1, 0
                0, 1, 2,
                2, 3, 0,
        };

        this->m_VertexDesc.vertices = std::move(std::vector<float>(vertices.begin(), vertices.end()));
        this->m_VertexDesc.textureCoords = std::move(std::vector<float>(texCoords.begin(), texCoords.end()));
        this->m_VertexDesc.normals = std::move(std::vector<float>(normals.begin(), normals.end()));
        this->m_VertexDesc.vertexCountPerIndex = 2;
        this->m_VertexDesc.setHasIndices(true);
        this->m_VertexDesc.indices = std::move(std::vector<Gfx_u32>(indices.begin(), indices.end()));

        auto mesh = std::make_shared<Mesh>(std::move(m_VertexDesc));
        mesh->m_Buffer.setProgram(m_Shader.getProgram()).initBuffer(BufferType::STATIC);
        mesh->m_TexturePaths["_Diffuse"] = path;

        glCheckError();

        if (this->m_Model == nullptr)
            this->m_Model = std::make_shared<Model>(path);

        this->m_Model->m_Meshes.push_back(std::move(mesh));
        this->m_Model->initTextures(m_Shader.getProgram());
        glCheckError();

        this->attachComponent(&m_Model->m_Meshes[0]->m_Texture, "texture");
        this->attachComponent(&m_Shader, "shader");
    } else {
        static std::array<float, 28> vertices = {
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
        };

        static std::array<Gfx_u32, 6> indices = {
                0, 1, 2,
                2, 3, 0,
        };

        this->m_VertexDesc.interleavedData = std::move(std::vector<float>(vertices.begin(), vertices.end()));
        this->m_VertexDesc.vertexCountPerIndex = 2;
        this->m_VertexDesc.setHasIndices(true);
        this->m_VertexDesc.indices = std::move(std::vector<Gfx_u32>(indices.begin(), indices.end()));
    }

    ALIEN_INFO("Sprite initialized");
}