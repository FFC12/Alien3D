#include "GameObject.h"
#include <Application.h>
#include <gfx/GfxBase.h>
#include <gfx/Primitives.h>

GameObject::GameObject(const std::string &name) : m_Name(name) {
    initGameObject();
}

GameObject::GameObject(const std::string &name, std::shared_ptr<Model> &model): m_Model(model) {

    initGameObject();
    m_Renderable = true;
}


GameObject::GameObject(const std::string &name, Primitive primitiveType) : m_Name(name) {
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

    m_Shader.createShader(::VertSrc, ::FragSrc);
    glCheckError();

    m_Shader.useProgram();
    glCheckError();

    for(auto mesh: m_Model->m_Meshes) {
        mesh->m_Buffer.setProgram(m_Shader.getProgram()).initBuffer(BufferType::STATIC);
    }

    m_Texture.generateTexture("../res/default.png", m_Shader.getProgram(), "_Diffuse");
    glCheckError();
}

void GameObject::drawCall() {
    if(m_Renderable) {
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

