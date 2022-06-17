#include <iostream>
#include <chrono>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Application.h>
#include <gfx/Buffer.h>
#include <gfx/Primitives.h>
#include <gfx/Texture.h>
#include <gfx/Shader.h>
#include <engine/Camera.h>

int main() {
    AlienApplication application(GfxDeviceType::GFX_OGL);

    ALIEN_INFO("Creating application...");
    if (!application.create(1920, 1080, "Alien3d")) {
        ALIEN_ASSERT2("FAILED");
        return 1;
    }

    Buffer<float> buffer;
    Shader shader;
    std::unique_ptr<Texture> texture;

    auto onInit = [&]() {
        shader.createShader(::VertSrc, ::FragSrc);
        auto shaderProgram = shader.getProgram();

        VertexDescriptor<float> cubeDescriptor(
                Primitives::Cube::Vertices,
                Primitives::Cube::Colors,
                Primitives::Cube::Normals,
                Primitives::Cube::TexCoord
        );

        buffer.setProgram(shaderProgram)
                .initBuffer(cubeDescriptor, BufferType::STATIC);
        buffer.useProgram();

        texture = std::make_unique<Texture>("../res/test.png", shaderProgram, "_MainTex");
        texture->generateTexture("../res/image.png", shaderProgram, "_BumpMap");
    };

    auto onUpdate = [&]() {
        texture->enableTexture();
        glBindVertexArray(buffer.getVAO());

        glm::mat4 model(1.0f);
        //model = glm::rotate(model, glm::radians(180.0f) * time, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setUniform4("model", model);
        shader.setUniform4("view", application.getWorldCamera().getViewMatrix());
        shader.setUniform4("proj", application.getWorldCamera().getProjMatrix());

        //glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glCheckError();
    };

    application.start(onInit, onUpdate);

    return 0;
}
