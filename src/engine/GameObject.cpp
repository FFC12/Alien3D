#include "GameObject.h"
#include <glad/glad.h>
#include "ShaderManager.h"
#include <Application.h>
#include <gfx/GfxBase.h>
#include <engine/Light.h>
#include <engine/SpriteAnimation.h>
#include <engine/PhysicsBody.h>
#include <engine/RenderQueue.h>
#include <engine/Scripting.h>
#include <list>

GameObject::GameObject(const std::string &name) {
    this->m_Name = name;

    initObject();
}

GameObject::GameObject(const std::string &name, std::shared_ptr<Model> &model) : m_Model(model) {
    this->m_Name = name;

    initObject();
    if (!m_IsSprite)
        initGameObject();
    m_Renderable = true;
}


GameObject::GameObject(const std::string &name, Primitive primitiveType) : m_Name(name) {
    this->m_Name = name;

    if (primitiveType == Primitive::CUBE) {
        // TODO: Cube primitive
    } else {
        return;
    }

    initGameObject();
    m_Renderable = true;
}

GameObject::~GameObject() {
    std::map<std::string, std::pair<Component *, bool>>::iterator it;
    std::list<std::map<std::string, std::pair<Component *, bool>>::iterator> iteratorList;

    for (it = m_Components.begin(); it != m_Components.end(); ++it) {
        if (it->second.second == false) {
            iteratorList.push_back(it);
        }
    }

    for (auto i: iteratorList) {
        switch (i->second.first->componentType) {
            case SPRITE_ANIM:
                delete (SpriteAnimation *) i->second.first;
                break;
            case PHYSICS_BODY:
                delete (PhysicsBody *) i->second.first;
                break;
            case TRANSFORM:
                delete (Transform *) i->second.first;
                break;
            case GAME_SCRIPT:
                delete (Scripting *) i->second.first;
                break;
            case SHADER:
                delete (Shader *) i->second.first;
                break;
            case TEXTURE:
                delete (Texture *) i->second.first;
                break;
            case NO_COMP:
                break;
        }

        m_Components.erase(i->first);
    }
}

void GameObject::initGameObject() {
    ShaderManager::getInstance().addIncludeFile("common.glsl.h", RESOURCE_PATH("shaders/common.glsl.h"));
    m_Shader = ShaderManager::getInstance().addShader("default", RESOURCE_PATH("shaders/default_vertex.glsl"),
                                                      RESOURCE_PATH("shaders/default_fragment.glsl"));

//    m_Shader.createShader(::VertSrc, ::FragSrc);
    glCheckError();

    m_Shader.useProgram();
    glCheckError();

    for (auto mesh: m_Model->m_Meshes) {
        mesh->m_Buffer.setProgram(m_Shader.getProgram()).initBuffer(BufferType::STATIC);
    }

    m_Model->initTextures(m_Shader.getProgram());
    glCheckError();
}

void GameObject::duplicate(const std::string &n, GameObject &o) {
//    GameObject* go = new GameObject(n);
//    go->m_Shader = o.m_Shader;
//    go->m_Texture = new Texture(o.m_Texture->getImagePath(),o.m_Shader.getProgram(),"_Diffuse");
}

void GameObject::attachComponent(const ComponentType &type) {
    auto checkComponent = [&](const std::string &name) -> bool {
        if (this->m_Components.count(name) > 0) {
            ALIEN_ERROR("This component has already added to GameObject!");
            return false;
        } else {
            return true;
        }
    };

    switch (type) {
        case SPRITE_ANIM:
            if (!this->m_IsSprite)
                abort();
            if (checkComponent("sprite_animation")) {
                this->m_Components["sprite_animation"] = {new SpriteAnimation(*dynamic_cast<Sprite *>(this)), false};
                this->m_Components["sprite_animation"].first->componentType = type;
            }
            break;
        case PHYSICS_BODY:
            if (!this->m_IsSprite)
                abort();
            if (checkComponent("physics_body")) {
                this->m_Components["physics_body"] = {new PhysicsBody(*dynamic_cast<Sprite *>(this), BodyType::Static),
                                                      false};
                this->m_Components["physics_body"].first->componentType = type;
            }
            break;
        case TRANSFORM:
            if (checkComponent("transform")) {
                this->m_Components["transform"] = {new Transform(), false};
                this->m_Components["transform"].first->componentType = type;
            }
            break;
        case GAME_SCRIPT:
            if (checkComponent("game_script")) {
                this->m_Components["game_script"] = {new Scripting(), false};
                this->m_Components["game_script"].first->componentType = type;
            }
            break;
        case SHADER:
            if (checkComponent("shader")) {
                this->m_Components["shader"] = {new Shader(), false};
                this->m_Components["shader"].first->componentType = type;
            }
            break;
        case TEXTURE:
            if (checkComponent("texture")) {
                this->m_Components["texture"] = {new Texture(), false};
                this->m_Components["texture"].first->componentType = type;
            }
            break;
        case NO_COMP:
            abort(); // ???
            break;
    }
}


void GameObject::attachComponent(const std::string &type) {
    auto checkComponent = [&](const std::string &name) -> bool {
        if (this->m_Components.count(name) > 0) {
            ALIEN_ERROR("This component has already added to GameObject!");
            return false;
        } else {
            return true;
        }
    };

    if (type == "sprite_animation") {
        if (!this->m_IsSprite)
            abort();
        if (checkComponent(type)) {
            this->m_Components[type] = {new SpriteAnimation(*dynamic_cast<Sprite *>(this)), false};
            this->m_Components[type].first->componentType = ComponentType::SPRITE_ANIM;
        }
    } else if (type == "physics_body") {
        if (!this->m_IsSprite)
            abort();
        if (checkComponent(type)) {
            this->m_Components[type] = {new PhysicsBody(*dynamic_cast<Sprite *>(this), BodyType::Static), false};
            this->m_Components[type].first->componentType = ComponentType::SPRITE_ANIM;
        }
    } else if (type == "transform") {
        if (checkComponent(type)) {
            this->m_Components[type] = {new Transform(), false};
            this->m_Components[type].first->componentType = ComponentType::SPRITE_ANIM;
        }
    } else if (type == "texture") {
        if (checkComponent(type)) {
            this->m_Components[type] = {new Texture(), false};
            this->m_Components[type].first->componentType = ComponentType::SPRITE_ANIM;
        }
    } else if (type == "shader") {
        if (checkComponent(type)) {
            this->m_Components[type] = {new Shader(), false};
            this->m_Components[type].first->componentType = ComponentType::SPRITE_ANIM;
        }
    } else if (type == "game_script") {
        if (checkComponent(type)) {
            this->m_Components[type] = {new Scripting(), false};
            this->m_Components[type].first->componentType = ComponentType::SPRITE_ANIM;
        }
    }
}

void GameObject::drawCall() {
    if (m_Renderable) {
        m_Shader.useProgram();

        m_Shader.setFloat("_Width", AlienApplication::WIDTH);
        m_Shader.setFloat("_Height", AlienApplication::HEIGHT);
        m_Shader.setFloat("_Time", AlienApplication::Time);

        if (m_IsSprite) {
            m_Shader.setBool("_HasLight", false);
            m_Shader.setBool("_IsSprite", true);

            auto transform = this->getComponent<Transform>("transform");
            transform->updateTransform(true);

            auto modelMatrix = transform->getModelMatrix();
            auto cameraPos = AlienApplication::Camera.getCameraPos();

            m_Shader.setUniform4("model", modelMatrix);
            m_Shader.setVector3("cameraPos", cameraPos);
            m_Shader.setBool("_Batch", false);

            // Animation calls
            auto anim = this->getComponent<SpriteAnimation>("sprite_animation");
            if (anim != nullptr) {
                if (AlienApplication::GameMode) {
                    anim->animate();
                }
            }
        } else {
            if (!Light::Lights.empty()) {
                m_Shader.setBool("_HasLight", true);

                for (auto &light: Light::Lights) {
                    // 1: point
                    // 2: sun
                    // 3: spot
                    switch (light->getLightType()) {
                        case PointLight: {
                            m_Shader.setInt("_LightType", 1);

                            auto lightPos = light->getLightPos();
                            m_Shader.setVector3("_PointLight.lightPos", glm::vec3(lightPos.x, lightPos.y, lightPos.z));

                            auto lightCol = light->getLightColor();
                            m_Shader.setVector3("_PointLight.color", glm::vec3(lightCol.x, lightCol.y, lightCol.z));

                            auto cameraPos = AlienApplication::Camera.getCameraPos();
                            m_Shader.setVector3("_PointLight.cameraPos", cameraPos);

                            break;
                        }
                        case SunLight:
                            m_Shader.setInt("_LightType", 2);
                            break;
                        case SpotLight:
                            m_Shader.setInt("_LightType", 3);
                            break;
                    }
                }
            } else {
                m_Shader.setBool("_HasLight", false);
            }

            auto transform = this->getComponent<Transform>("transform");
            transform->updateTransform(false);

            auto modelMatrix = transform->getModelMatrix();
            auto viewMatrix = AlienApplication::Camera.getViewMatrix();
            auto projMatrix = AlienApplication::Camera.getProjMatrix();

            m_Shader.setUniform4("model", modelMatrix);
            m_Shader.setUniform4("view", viewMatrix);
            m_Shader.setUniform4("proj", projMatrix);

        }

        if (WireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        m_Model->renderGL();

        if (!m_GameScripts.empty()) {

        }
    }
}

void GameObject::GameobjectWidget() {
    ImGui::Checkbox("Wireframe Mode", &WireframeMode);

    std::string s = "Scene List ( " + std::to_string(GameobjectList.size()) + " )";
    if (ImGui::TreeNode(s.c_str())) {
        static ImGuiTreeNodeFlags baseFlags =
                ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool dragAndDrop = false;
        static int selection_mask = (1 << 2);

        static std::string nodeClicked;

        for (auto gameobject: GameobjectList) {
            ImGuiTreeNodeFlags nodeFlags = baseFlags;

            if (nodeClicked == gameobject.first)
                nodeFlags |= ImGuiTreeNodeFlags_Selected;

            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
            auto firstPart = gameobject.first.substr(0, gameobject.first.find(":"));
            ImGui::TreeNodeEx(firstPart.c_str(), nodeFlags, "%s", firstPart.c_str());

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                nodeClicked = gameobject.first;

            }

            if (dragAndDrop && ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
        }

        ImGui::TreePop();

        if (GameobjectList.count(nodeClicked) > 0) {
            auto gameObject = GameobjectList.find(nodeClicked);

            auto name = gameObject->second->m_Name;
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::Button("Duplicate")) {
                    if (gameObject->second->m_IsSprite) {
                        Sprite *sprite = new Sprite(*gameObject->second->m_Sprite);
                    }
                }

                if (ImGui::Button("Delete")) {
                    if (gameObject->second->m_IsSprite) {
                        auto id = gameObject->second->m_Name + ":" + gameObject->second->m_ObjectUUID;

                        RenderQueue::getInstance().deleteQueue(*gameObject->second);
                        
                        if (Sprite::SpriteList.count(id) > 0) {
                            auto pos = Sprite::SpriteList.find(id);
                            delete pos->second;
                            Sprite::SpriteList.erase(pos);
                            nodeClicked = "";
                            goto delete_op;
                        } else {
                            ALIEN_ERROR("Potential Memory Leak Might Be Happened!");
                        }
                    }
                }

                ImGui::Text("Edit name (not working):");
                ImGui::InputText("##edit", (char *) name.c_str(), name.size() + 1);
                if (ImGui::Button("Close")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }


            static bool isOpen = true;
            if (!ImGui::Begin("Components", &isOpen, ImGuiWindowFlags_None)) {
                ImGui::End();
                return;
            }

            gameObject->second->components();

            static bool attachComponent = false;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.5f, 0.3f, 0.6f, 1.0f});
            if (ImGui::Button("Attach Component")) {
                attachComponent = true;
            }
            ImGui::PopStyleColor();

            if (attachComponent) {
                const char *componentNames[] = {"Sprite Animation", "Physics Body", "Transform", "Game Script",
                                                "Shader", "Texture"};
                static char buffer[128];

                static int itemCurrentIdx = 0;
                const char *comboPreviewValue = componentNames[itemCurrentIdx];
                if (ImGui::BeginCombo("Components", comboPreviewValue, 0)) {
                    for (int n = 0; n < IM_ARRAYSIZE(componentNames); n++) {
                        const bool isSelected = (itemCurrentIdx == n);

                        if (ImGui::Selectable(componentNames[n], isSelected))
                            itemCurrentIdx = n;

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                std::string path;
                if (itemCurrentIdx == (int) ComponentType::GAME_SCRIPT) {
                    ImGui::InputText("Script Name", &buffer[0], IM_ARRAYSIZE(buffer));
                    auto n = std::strlen(buffer);
                    path = std::string(buffer, n);
                }

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.2f, 0.6f, 1.0f});
                if (ImGui::Button("Done")) {
                    if (itemCurrentIdx == (int) ComponentType::GAME_SCRIPT) {
                        ALIEN_INFO("Done: " + RESOURCE_PATH(path));
                        auto pos = path.find_last_of('.');
                        auto fileName = path.substr(0, pos);
                        gameObject->second->attachComponent("game_script");

                        auto script = gameObject->second->getComponent<Scripting>("game_script");
                        auto scriptPath = RESOURCE_PATH("scripts/" + path);
                        script->setPath(scriptPath);
                        auto scriptData = script->loadScriptFromFile(scriptPath);
                        script->exec(scriptData);

                        attachComponent = false;
                        std::memset(&buffer[0], 0, sizeof(buffer));
                    } else {
                        gameObject->second->attachComponent((ComponentType) itemCurrentIdx);
                    }
                }

                ImGui::PopStyleColor();
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.6f, 0.2f, 0.2f, 1.0f});
                if (ImGui::Button("Cancel")) {
                    attachComponent = false;
                }
                ImGui::PopStyleColor();
            }
            delete_op:
            ImGui::End();
        }
    }
}

void GameObject::components() {
    for (auto iter = m_Components.rbegin(); iter != m_Components.rend(); ++iter) {
        iter->second.first->OnComponentWidgetDrawn();
    }
}

void GameObject::initObject() {
    this->m_ObjectUUID = Utils::getUUID();

    this->m_Components["transform"] = {new Transform(), false};

    auto objectName = this->m_Name + ":" + this->m_ObjectUUID;

    if (!WidgetInitialized) {
        COMPONENT_WIDGET_REGISTRY_S(objectName, GameObject::GameobjectWidget);
        WidgetInitialized = true;

    }

    if (GameobjectList.count(objectName) > 0) {
        abort();
    } else {
        GameobjectList[objectName] = this;
    }

    RenderQueue::getInstance().addQueue(this);
}

static std::unique_ptr<GameObject> Create(const std::string &name) {
    return std::move(std::make_unique<GameObject>(name));
}

void GameObject::QuickCreate(const std::string &name) {
    GameobjectList.insert(std::pair<std::string, GameObject *>(name, new GameObject(name)));
}