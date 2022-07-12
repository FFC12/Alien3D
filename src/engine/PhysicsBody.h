#ifndef ALIEN3D_PHYSICSBODY_H
#define ALIEN3D_PHYSICSBODY_H

#include <memory>
#include <Base.hpp>
#include <engine/Vector.h>
#include <engine/Component.h>
#include <engine/Physics2D.h>
#include "Sprite.h"

enum BodyType {
    Static = 0,
    Dynamic = 1,
    Kinematic = 2,
};

struct PhysicalMaterial {
    float friction;
    float density;
};

class PhysicsBody : public Component {
public:
    PhysicsBody(std::shared_ptr<Sprite> &sprite, const BodyType &bodyType,
                const PhysicalMaterial &physicalMaterial = {0.0f, 0.0f})
            : m_BodyType(bodyType) {

        auto transform = sprite->getComponent<Transform>("transform");
        m_Position = transform->getPosition();
        m_Angle = transform->getRotation().z;
        m_Bounds = transform->getScale();
        m_Bounds.y *= ((float) AlienApplication::HEIGHT / (float) AlienApplication::WIDTH) - 0.3f;

        b2BodyType bt;
        switch (bodyType) {
            case Static:
                bt = b2_staticBody;
                break;
            case Dynamic:
                bt = b2_dynamicBody;
                break;
            case Kinematic:
                bt = b2_kinematicBody;
                break;
        }

        m_BodyDef.type = bt;
        m_BodyDef.position.Set(m_Position.x, m_Position.y);
        m_BodyDef.angle = m_Angle;
        m_BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(sprite.get());// reinterpret_cast<uintptr_t>(sprite->getObjectUUID().c_str());
        m_Body = WorldSimulation::getInstance().m_World->CreateBody(&m_BodyDef);

        if (bt != b2_staticBody) {
            m_Fixture.friction = physicalMaterial.friction;
            m_Fixture.density = physicalMaterial.density;
        }

        m_Shape.SetAsBox(m_Bounds.x, m_Bounds.y);
        if (bt == b2_staticBody) {
            m_Body->CreateFixture(&m_Shape, 0.0f);
        } else {
            m_Fixture.shape = &m_Shape;
            m_Body->CreateFixture(&m_Fixture);
        }
    }

    void applyForce(const Vector3 &f, const Vector3 &c) {
        static b2Vec2 force, center;
        force.x = f.x;
        force.y = f.y;

        center.x = c.x;
        center.y = c.y;

        if (m_Body != nullptr) {
            m_Body->ApplyForce(force, center, true);
        }
    }

    void applyForce(const Vector3 &f, float m) {
        if (m_Body != nullptr) {
            auto center = m_Body->GetWorldCenter();
            Vector3 forceDirection = Vector3(f.x * m, f.y * m, 0.0f);
            applyForce(forceDirection, Vector3(center.x, center.y, 0.0f));
        }
    }

    void applyForceImpulsive(const Vector3 &f, const Vector3 &c) {
        static b2Vec2 force, center;
        force.x = f.x;
        force.y = f.y;

        center.x = c.x;
        center.y = c.y;

        if (m_Body != nullptr) {
            m_Body->ApplyLinearImpulse(force, center, true);
        }
    }

    void applyForceImpulsive(const Vector3 &f, float m) {
        if (m_Body != nullptr) {
            auto center = m_Body->GetWorldCenter();
            Vector3 forceDirection = Vector3(f.x * m, f.y * m, 0.0f);
            applyForceImpulsive(forceDirection, Vector3(center.x, center.y, 0.0f));
        }
    }

    Vector3 getLinearVelocity() {
        if (m_Body != nullptr) {
            auto v = m_Body->GetLinearVelocity();
            return {v.x, v.y, 0.f};
        } else {
            return {0.0f, 0.0f, 0.0f};
        }
    }

    void OnComponentWidgetDrawn() override {
        physics2DWidget();
    }

private:
    void physics2DWidget() {
        if (ImGui::TreeNode("Physics Body")) {
            if (ImGui::TreeNode("Body Type")) {
                static const char *p[3] = {"Static", "Dynamic", "Kinematic"};
                for (int n = 0; n < 3; n++) {
                    if (ImGui::Selectable(p[n], (int) m_BodyType == n)) {
                        m_BodyType = (BodyType) n;

                        b2BodyType bt;
                        switch (m_BodyType) {
                            case Static:
                                bt = b2_staticBody;
                                break;
                            case Dynamic:
                                bt = b2_dynamicBody;
                                break;
                            case Kinematic:
                                bt = b2_kinematicBody;
                                break;
                        }

                        if (m_Body->GetType() != bt) {
                            m_Body->SetType(bt);
                        }
                    }
                }

                ImGui::TreePop();
            }

            ImGui::Separator();

            auto lv = m_Body->GetLinearVelocity();
            ImGui::Text("Linear Velocity: (%f , %f)", lv.x, lv.y);

            auto av = m_Body->GetAngularVelocity();
            ImGui::Text("Angular Velocity: %f", av);

            auto pos = m_Body->GetPosition();
            ImGui::Text("Position: (%.9g, %.9g)", pos.x, pos.y);

            auto angle = m_Body->GetAngle();
            ImGui::Text("Angle: %.9g", angle);

            auto ld = m_Body->GetLinearDamping();
            ImGui::Text("Linear Damping: %.9g", ld);

            auto ad = m_Body->GetAngularVelocity();
            ImGui::Text("Angular Damping: %.9g", ad);
//            ImGui::Text("bd.allowSleep = bool(%d);\n", m_flags & e_autoSleepFlag);
//            ImGui::Text("bd.awake = bool(%d);\n", m_flags & e_awakeFlag);
//            ImGui::Text("bd.fixedRotation = bool(%d);\n", m_flags & e_fixedRotationFlag);

            auto grav = m_Body->GetGravityScale();
            ImGui::Text("Gravity Scale: %.9g", grav);

            ImGui::Separator();
            ImGui::TreePop();
        }
    }

    Vector3 m_Bounds;
    Vector3 m_Position;
    float m_Angle;
    BodyType m_BodyType;

    b2Body *m_Body;
    b2BodyDef m_BodyDef;
    b2FixtureDef m_Fixture;
    b2PolygonShape m_Shape;
};

#endif //ALIEN3D_PHYSICSBODY_H
