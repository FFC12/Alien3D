#include "GameObject.h"
#include <Application.h>
#include <gfx/GfxBase.h>
#include <gfx/Primitives.h>

GameObject::GameObject(const std::string &name) {
    this->m_Name = name;

    initGameObject();
}

GameObject::GameObject(const std::string &name, std::shared_ptr<Model> &model) : m_Model(model) {
    this->m_Name = name;

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
    this->m_ObjectUUID = Utils::getUUID();

    this->m_Transformation = std::make_shared<Transform>();
    this->m_Components[typeid(Transform).name()] = std::reinterpret_pointer_cast<Component>(this->m_Transformation);

    auto objectName = this->m_Name + ":" + this->m_ObjectUUID;

    if (!WidgetInitialized) {
        COMPONENT_WIDGET_REGISTRY_S(objectName, GameObject::GameobjectWidget);
        WidgetInitialized = true;
    }

    GameobjectList[objectName] = std::make_shared<GameObject>(*this);

    m_Shader.createShader(::VertSrc, ::FragSrc);
    glCheckError();

    m_Shader.useProgram();
    glCheckError();

    for (auto mesh: m_Model->m_Meshes) {
        mesh->m_Buffer.setProgram(m_Shader.getProgram()).initBuffer(BufferType::STATIC);
    }

    m_Model->initTextures(m_Shader.getProgram());
    glCheckError();
}

void GameObject::drawCall() {
    if (m_Renderable) {
        m_Shader.useProgram();

        ALIEN_ASSERT(m_Transformation != nullptr);

        m_Transformation->updateTransform();

        auto modelMatrix = m_Transformation->getModelMatrix();
        auto viewMatrix = AlienApplication::Camera.getViewMatrix();
        auto projMatrix = AlienApplication::Camera.getProjMatrix();

        m_Shader.setUniform4("model", modelMatrix);
        m_Shader.setUniform4("view", viewMatrix);
        m_Shader.setUniform4("proj", projMatrix);

        m_Model->renderGL();
    }
}

void GameObject::GameobjectWidget() {
    if(ImGui::TreeNode("Scene List")) {
        static ImGuiTreeNodeFlags baseFlags =
                ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool dragAndDrop = false;
        static int selection_mask = (1 << 2);

        static std::string nodeClicked;
        for (auto gameobject: GameobjectList) {
            ImGuiTreeNodeFlags nodeFlags = baseFlags;

            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
            auto firstPart = gameobject.first.substr(0, gameobject.first.find(":"));
            ImGui::TreeNodeEx(gameobject.first.c_str(), nodeFlags, "%s", firstPart.c_str());

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                nodeClicked = gameobject.first;

            if (dragAndDrop && ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
        }

        ImGui::TreePop();

        if(GameobjectList.count(nodeClicked) > 0){
            static bool isOpen = true;
            if(!ImGui::Begin("Transformation Window", &isOpen, ImGuiWindowFlags_None)) {
                ImGui::End();
                return;
            }

            GameobjectList[nodeClicked]->m_Transformation->transformWidget();

            ImGui::End();
        }
    }
}