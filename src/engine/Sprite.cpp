#include "Sprite.h"
#include <Application.h>
#include "ShaderManager.h"
#include <engine/SpriteAnimation.h>
#include <engine/PhysicsBody.h>

Sprite::Sprite(const std::string &name) : GameObject(name) {
    AlienApplication::Camera.setOrthoCamera(true);
    AlienApplication::Camera.setPosition(Vector3(0.0f, 0.0f, 0.0f));

    if (!Warning) {
        ALIEN_INFO("Ortho camera is enabled since you created a sprite! *you supposed to be aware* ");
        Warning = true;
    }
}

Sprite::Sprite(const std::string &name, bool alpha) : GameObject(name), m_AlphaTrans(alpha) {
    AlienApplication::Camera.setOrthoCamera(true);
    AlienApplication::Camera.setPosition(Vector3(0.0f, 0.0f, 0.0f));

    if (!Warning) {
        ALIEN_INFO("Ortho camera is enabled since you created a sprite! *you supposed to be aware* ");
        Warning = true;
    }

    // by default image
}

Sprite::Sprite(const std::string &name, const std::string &path, bool alpha) : GameObject(name),
                                                                               m_ImagePath(path),
                                                                               m_AlphaTrans(alpha) {
    AlienApplication::Camera.setOrthoCamera(true);
    AlienApplication::Camera.setPosition(Vector3(0.0f, 0.0f, 0.0f));

    if (!Warning) {
        ALIEN_INFO("Ortho camera is enabled since you created a sprite! *you supposed to be aware* ");
        Warning = true;
    }

    initSprite(name, path);
}

Sprite::~Sprite() {
    auto id = this->m_Name + ":" + this->m_ObjectUUID;
    if (GameobjectList.count(id) > 0) {
        auto pos = GameobjectList.find(id);
        GameobjectList.erase(pos->first);
    } else {
        ALIEN_ERROR("Potential Memory Leak Might Be Happened!");
    }
}

void Sprite::initSprite(const std::string &name, const std::string &path) {
    this->m_Renderable = true;
    this->m_IsSprite = true;
    this->m_ImagePath = path;

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

    this->m_Sprite = this;
    SpriteList[name + ":" + m_ObjectUUID] = this;
    this->m_Texture = &m_Model->m_Meshes[0]->m_Texture;
    this->attachComponent(&m_Model->m_Meshes[0]->m_Texture, "texture", true);
    this->attachComponent(&m_Shader, "shader", true);
}

void Sprite::duplicate(const std::string &n, Sprite &o) {

//    GameObject::duplicate(n, o);
}

Sprite::Sprite(const Sprite &sprite) : GameObject(sprite.m_Name) {
    this->m_VertexDesc = sprite.m_VertexDesc;
    //this->m_ObjectUUID = Utils::getUUID();
    this->m_Name = sprite.m_Name;
    this->m_AlphaTrans = sprite.m_AlphaTrans;
    this->m_IsSprite = sprite.m_IsSprite;
    this->m_Renderable = sprite.m_Renderable;

    for (auto &component: sprite.m_Components) {
        auto type = component.second.first->componentType;
        attachComponent(type);

        switch (type) {
            case SPRITE_ANIM: {
                // Copy sprite animation by hand since (using a copy constructor is not possible)
                auto c = getComponent<SpriteAnimation>("sprite_animation");
                auto o = (SpriteAnimation *) component.second.first;

                c->setWidth(o->getWidth());
                c->setHeight(o->getHeight());
                c->setFrameX(o->getFrameX());
                c->setFrameY(o->getFrameY());

                break;
            }
            case PHYSICS_BODY: {
                auto c = getComponent<PhysicsBody>("physics_body");
                auto o = (PhysicsBody *) component.second.first;

                c->setAngle(o->getAngle());
                c->setBodyType(o->getBodyType());
                c->setBounds(o->getBounds());
                c->setIsAllowSleep(o->getIsAllowSleep());
                c->setBoundsCenter(o->getBoundsCenter());
                break;
            }
            case TRANSFORM: {
                auto c = getComponent<Transform>("transform");
                auto o = (Transform*)component.second.first;

                c->setPosition(o->getPosition());
                c->setRotation(o->getRotation());
                c->setScale(o->getScale());
                break;
            }
            case GAME_SCRIPT:
                break;
            case SHADER:
                //TODO: Copy shader
                break;
            case TEXTURE:
                //TODO: Copy texture
                break;
            case NO_COMP:
                break;
        }

    }

    initSprite(sprite.m_Name, sprite.m_ImagePath);
}


