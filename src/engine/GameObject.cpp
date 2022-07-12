#include "GameObject.h"
#include "ShaderManager.h"
#include <Application.h>
#include <gfx/GfxBase.h>
#include <engine/Light.h>
#include <engine/SpriteAnimation.h>
#include <engine/PhysicsBody.h>
#include <engine/RenderQueue.h>

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

//    m_Components["shader"] = std::shared_ptr<Shader>(&m_Shader);
//    m_Components["texture"] = std::shared_ptr<Texture>(&m_Texture);
}

void GameObject::drawCall() {
    if (m_Renderable) {
        ALIEN_ASSERT(m_Transformation != nullptr);
        m_Shader.useProgram();

        m_Shader.setFloat("_Time", AlienApplication::Time);

        if (m_IsSprite) {
            m_Shader.setBool("_HasLight", false);
            m_Shader.setBool("_IsSprite", true);

            m_Transformation->updateTransform(true);

            auto modelMatrix = m_Transformation->getModelMatrix();
            auto cameraPos = AlienApplication::Camera.getCameraPos();

            m_Shader.setUniform4("model", modelMatrix);
            m_Shader.setVector3("cameraPos", cameraPos);
            m_Shader.setBool("_Batch", false);

            // Animation calls
            auto anim = this->getComponent<SpriteAnimation>("sprite_animation");
            if (anim != nullptr) {
                anim->animate();
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

            m_Transformation->updateTransform(false);

            auto modelMatrix = m_Transformation->getModelMatrix();
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
//            static char name[32] = "Label1";
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::Button("Duplicate")) {

                }

                if (ImGui::Button("Delete")) {

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
//            GameobjectList[nodeClicked]->m_Transformation->transformWidget();

            if (gameObject->second->m_IsSprite) {

            } else {

            }

            ImGui::End();
        }
    }
}


void GameObject::components() {
    for (auto iter = m_Components.rbegin(); iter != m_Components.rend(); ++iter) {
        iter->second->OnComponentWidgetDrawn();
    }
}

void GameObject::initObject() {
    this->m_ObjectUUID = Utils::getUUID();

    this->m_Transformation = std::make_shared<Transform>();
    this->m_Components["transform"] = this->m_Transformation.get();

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
