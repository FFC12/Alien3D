#ifndef ALIEN3D_PHYSICS2D_H
#define ALIEN3D_PHYSICS2D_H

#include <box2d/box2d.h>
#include "Base.hpp"
#include "GameObject.h"
#include <engine/Sprite.h>
#include <engine/DebugDraw.h>

class PhysicsBody;

struct SimulationConfig {
    f32 gravityY{-9.8f};
    f32 gravityX{0.0f};
    f32 stepCount{300.0f};

    i32 velocityIter{10};
    i32 positionIter{20};

    bool dontLetSleep{false};
    bool debugPhysics{false};
};

class WorldSimulation {
    friend class PhysicsBody;

public:
    static WorldSimulation &getInstance() {
        static WorldSimulation ins;
        return ins;
    }

    void step(bool);

    void debugDrawRender() {
        //GL
        if (m_DebugPhysics) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(m_DebugDrawID);

            setCameraPos();
            glCheckError();

            m_Shader.setUniform4("M", m_Model);
//            glUniformMatrix4fv(m_MVPLoc, 1, GL_FALSE, m_MVPPtr);
            glCheckError();
            m_DebugDraw->Render();
        }
    }

    void setSimulationConfig(const SimulationConfig &config) {
        this->m_GravityX = config.gravityX;
        this->m_GravityY = config.gravityY;
        this->m_StepCount = config.stepCount;
        this->m_VelocityIter = config.velocityIter;
        this->m_PositionIter = config.positionIter;
    }

private:
    WorldSimulation();

    void setCameraPos();

    void worldSimulationWidget() {
        if (ImGui::Begin("World Simulation")) {

            if (ImGui::TreeNode("Stats")) {
                ImGui::Text("Total Body Count: %d", m_World->GetBodyCount());
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Settings")) {
                ImGui::DragFloat("Gravity X", &this->m_GravityX);
                ImGui::DragFloat("Gravity Y", &this->m_GravityY);
                ImGui::DragFloat("Step Count", &this->m_StepCount);
                ImGui::DragInt("Velocity Iteration", &this->m_VelocityIter, 1.0f, 0, 50);
                ImGui::DragInt("Position Iteration", &this->m_PositionIter, 1.0f, 0, 50);
                ImGui::Checkbox("Debug Physics", &this->m_DebugPhysics);

                if (this->m_DebugPhysics) {
                    m_World->SetDebugDraw(m_DebugDraw.get());
                } else {
                    m_World->SetDebugDraw(nullptr);
                }

                ImGui::TreePop();
            }

            ImGui::End();
        }
    }

    f32 m_GravityX, m_GravityY = -10.0f;
    f32 m_StepCount{300.0f};
    i32 m_VelocityIter{10};
    i32 m_PositionIter{20};
    i32 m_BodyCount{0};
    bool m_DebugPhysics{false};

    // Debug Draw
    Shader m_Shader;
    std::shared_ptr<b2draw::DebugDraw> m_DebugDraw;
    Gfx_u32 m_DebugDrawID;
    glm::mat4 m_Model;

    std::shared_ptr<b2World> m_World;
};

#endif //ALIEN3D_PHYSICS2D_H

