#include "Physics2D.h"
#include <Application.h>

void WorldSimulation::step() {
    if (m_World != nullptr) {

        // make this more efficient since it's keep copying b2vec2
        ALIEN_INFO("Graivit y at impl: " << m_GravityY);
        m_World->SetGravity(b2Vec2(m_GravityX, m_GravityY));

        if (m_World->GetBodyCount() > 0) {
            if (this->m_DebugPhysics)
                this->m_BodyCount = m_World->GetBodyCount();

            for (i32 i = 0; i < 30; i++) {
                auto bodies = m_World->GetBodyList();

//                m_World->m_DebugDraw();
                auto body = bodies;
                while (body != nullptr) {
                    auto pos = body->GetPosition();
                    auto angle = body->GetAngle();

                    auto *sprite = reinterpret_cast<Sprite *>(body->GetUserData().pointer);
                    auto transform = sprite->getComponent<Transform>("transform");
                    if (body->GetType() != b2_staticBody) {
                        transform->setPosition(Vector3(pos.x, pos.y, 0.0f));
                        transform->setRotation(angle);
                    }

                    if (m_DontLetSleep)
                        body->SetAwake(true);
                    else
                        body->SetAwake(false);

                    body = body->GetNext();
                }

                m_World->Step(1.0f / m_StepCount, m_VelocityIter, m_PositionIter);
                m_World->ClearForces();

                if (this->m_DebugPhysics) {
                    m_DebugDraw->Clear();
                    m_World->DebugDraw();
                    m_DebugDraw->BufferData();
                }
            }
        }
    }
}


WorldSimulation::WorldSimulation() {
    b2Vec2 gravity(0.0f, -10.0f);

    // Create OpenGL shaders and program.
    std::string const vertSrc = R"(
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 colour;
out vec4 fsColour;
uniform mat4 MVP;
void main() {
	gl_Position =  MVP * vec4(position.x, position.y, 0, 1);
	fsColour = colour;
}
	)";

    std::string const fragSrc = R"(
#version 330 core
in vec4 fsColour;
out vec4 fragColour;
void main() {
	fragColour = vec4(fsColour.xyz,0.1);
}
    )";
    m_Shader.createShader(vertSrc, fragSrc);
    m_DebugDrawID = m_Shader.getProgram();

    auto model = glm::mat4(1.0f);
    auto view = AlienApplication::Camera.getViewMatrix();
    auto proj = AlienApplication::Camera.getProjMatrix();

    static glm::mat4x4 const MVP = proj * view * model;
    m_MVPPtr = &MVP[0][0];
    m_MVPLoc = glGetUniformLocation(m_DebugDrawID, "MVP");

    m_DebugDraw = std::make_shared<b2draw::DebugDraw>(
            glGetAttribLocation(m_DebugDrawID, "position"),
            glGetAttribLocation(m_DebugDrawID, "colour"),
            16, 0.01f, 4.0f);

    m_DebugDraw->SetFlags(b2Draw::e_shapeBit);
    m_World = std::make_shared<b2World>(gravity);
    m_World->SetDebugDraw(m_DebugDraw.get());
    m_World->SetDebugDraw(nullptr);

    COMPONENT_WIDGET_REGISTRY("world_simulation", WorldSimulation::worldSimulationWidget);
}
