#include "GameObject.h"
#include <Application.h>
#include <gfx/GfxBase.h>
#include <gfx/Primitives.h>

GameObject::GameObject(const std::string &name) : m_Name(name) {}

GameObject::GameObject(const std::string &name, Primitive primitiveType) : m_Name(name) {
    this->m_ObjectUUID = Utils::getUUID();

    this->m_Transformation = std::make_shared<Transform>();
    this->m_Components[typeid(Transform).name()] = std::reinterpret_pointer_cast<Component>(this->m_Transformation);

    if (primitiveType == Primitive::CUBE) {
        VertexDescriptor<f32> cubeDescriptor(
                Primitives::Cube::Vertices,
                Primitives::Cube::Colors,
                Primitives::Cube::Normals,
                Primitives::Cube::TexCoord
        );

        m_Shader.createShader(::VertSrc, ::FragSrc);
        glCheckError();
        m_Buffer.setProgram(m_Shader.getProgram())
            .initBuffer(cubeDescriptor, BufferType::STATIC);
        glCheckError();
        m_Shader.useProgram();
        glCheckError();
        m_Texture.generateTexture("../res/test.png", m_Shader.getProgram(), "_MainTex");
        glCheckError();
        m_Renderable = true;
    }
}

void GameObject::drawCall() {
    m_Shader.useProgram();
    m_Texture.enableTexture();
    m_Buffer.enableVAO();

    ALIEN_ASSERT(m_Transformation != nullptr);

    m_Transformation->updateTransform();

    auto modelMatrix = m_Transformation->getModelMatrix();
    auto viewMatrix = AlienApplication::Camera.getViewMatrix();
    auto projMatrix = AlienApplication::Camera.getProjMatrix();

    m_Shader.setUniform4("model", modelMatrix);
    m_Shader.setUniform4("view", viewMatrix);
    m_Shader.setUniform4("proj", projMatrix);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glCheckError();

}
