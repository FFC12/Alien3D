#ifndef ALIEN3D_PHYSICS2D_H
#define ALIEN3D_PHYSICS2D_H

#include <box2d/box2d.h>
#include "Base.hpp"
#include "GameObject.h"
#include <engine/Sprite.h>
#include <b2draw/DebugDraw.h>

class PhysicsBody;

class WorldSimulation {
    friend class PhysicsBody;

public:
    static WorldSimulation &getInstance() {
        static WorldSimulation ins;
        return ins;
    }

    void step();

    void debugDrawRender() {
        //GL
        if (m_DebugPhysics) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(m_DebugDrawID);
            glCheckError();
            glUniformMatrix4fv(m_MVPLoc, 1, GL_FALSE, m_MVPPtr);
            glCheckError();
            m_DebugDraw->Render();
        }
    }

private:
    WorldSimulation();

    void worldSimulationWidget() {
        if (ImGui::Begin("World Simulation")) {

            if (ImGui::TreeNode("Stats")) {
                ImGui::Text("Total Body Count: %d", m_World->GetBodyCount());
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Settings")) {
                ImGui::DragFloat("Gravity X", &this->m_GravityX);
                ImGui::DragFloat("Gravity Y", &this->m_GravityY);
                ALIEN_INFO("Graivit y at first place: " << m_GravityY);
                ImGui::DragFloat("Step Count", &this->m_StepCount);
                ImGui::DragInt("Velocity Iteration", &this->m_VelocityIter, 1.0f, 0, 50);
                ImGui::DragInt("Position Iteration", &this->m_PositionIter, 1.0f, 0, 50);
                ImGui::Checkbox("Debug Physics", &this->m_DebugPhysics);
                ImGui::Checkbox("Don't Let Sleep", &this->m_DontLetSleep);

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
    bool m_DontLetSleep {false};
    bool m_DebugPhysics{false};

    // Debug Draw
    Shader m_Shader;
    std::shared_ptr<b2draw::DebugDraw> m_DebugDraw;
    Gfx_u32 m_DebugDrawID;
    const Gfx_f32 *m_MVPPtr;
    Gfx_i32 m_MVPLoc;

    std::shared_ptr<b2World> m_World;
};

#endif //ALIEN3D_PHYSICS2D_H

