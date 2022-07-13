#include "Physics2D.h"
#include <Application.h>

void WorldSimulation::step() {
    if (m_World != nullptr) {
        if (m_World->GetBodyCount() > 0) {
            for (i32 i = 0; i < 30; i++) {
                auto bodies = m_World->GetBodyList();

//                m_World->DebugDraw();
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
                    body = body->GetNext();
                }

                m_World->Step(1.0f / 300.0f, 10, 20);
                m_World->ClearForces();
            }
        }
    }
}


WorldSimulation::WorldSimulation() {
    b2Vec2 gravity(0.0f, -10.0f);
    m_World = std::make_shared<b2World>(gravity);
    m_World->SetDebugDraw(&m_DebugDraw);
    m_DebugDraw.SetFlags(b2Draw::e_shapeBit);
}
