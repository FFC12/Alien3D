#ifndef ALIEN3D_PHYSICSBODY_H
#define ALIEN3D_PHYSICSBODY_H

#include <memory>
#include <Base.hpp>
#include <engine/Vector.h>
#include <engine/Component.h>
#include <engine/Physics2D.h>
#include "Sprite.h"
#include <Application.h>
#include <utils/Helper.h>
#include <functional>
#include <engine/SceneManager.h>

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
    PhysicsBody(Sprite &sprite, const BodyType &bodyType,
                const PhysicalMaterial &physicalMaterial = {0.1f, 1.0f})
            : m_BodyType(bodyType) {
        this->componentType = PHYSICS_BODY;

        auto transform = sprite.getComponent<Transform>("transform");
        transform->registerCallback([&](const Vector3 &p, const Vector3 &r, const Vector3 &s) {
            transformEventHandler(p, r, s);
        });

        m_Position = transform->getPosition();
        m_Angle = transform->getRotation().z * (float) (M_PI / 180);
        auto scale = transform->getScale();
        m_Bounds = Vector3(scale.x, scale.y, 0.5f);

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
        m_BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&sprite);// reinterpret_cast<uintptr_t>(sprite->getObjectUUID().c_str());
        m_Body = WorldSimulation::getInstance().m_World->CreateBody(&m_BodyDef);
        m_Body->SetTransform(b2Vec2(m_Position.x, m_Position.y), m_Angle);

        // When deserialized, this is what it needs.
        m_Body->SetAwake(!m_AllowSleep);

        if (bt != b2_staticBody) {
            m_Fixture.friction = physicalMaterial.friction;
            m_Fixture.density = physicalMaterial.density;
        }

        b2Vec2 vertices[4];
        vertices[0] = b2Vec2(-m_Bounds.x + m_BoundsCenter.x, -m_Bounds.y + m_BoundsCenter.y);
        vertices[1] = b2Vec2(m_Bounds.x + m_BoundsCenter.x, -m_Bounds.y + m_BoundsCenter.y);
        vertices[2] = b2Vec2(m_Bounds.x + m_BoundsCenter.x, m_Bounds.y + m_BoundsCenter.y);
        vertices[3] = b2Vec2(-m_Bounds.x + m_BoundsCenter.x, m_Bounds.y + m_BoundsCenter.y);

        m_Shape.Set(&vertices[0], 4);
//        m_Shape.SetAsBox(m_Bounds.x, m_Bounds.y);
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

    Vector3 getBoundsCenter() {
        return m_BoundsCenter;
    }

    void setBoundsCenter(Vector3 c) {
        m_BoundsCenter = c;
    }

    Vector3 getBounds(){
        return m_Bounds;
    }

    void setBounds(Vector3 b) {
        m_Bounds = b;
    }

    Vector3 getSimulationPosition() {
        return m_Position;
    }

    void setSimulationPosition(Vector3 p) {
        m_Position = p;
    }

    BodyType getBodyType() {
        return m_BodyType;
    }

    void setBodyType(BodyType b) {
        m_BodyType = b;
    }

    bool getIsAllowSleep() const {
        return m_AllowSleep;
    }

    void setIsAllowSleep(bool s) {
        m_AllowSleep = s;
    }

    f32 getAngle() const {
        return m_Angle;
    }

    void setAngle(f32 a) {
        m_Angle = a;
    }

    b2BodyDef getBodyDef() {
        return m_BodyDef;
    }

    b2FixtureDef getBodyFixtureDef() {
        return m_Fixture;
    }

    b2PolygonShape getShape() {
        return m_Shape;
    }

    void OnComponentWidgetDrawn() override {
        physics2DWidget();
    }

    void OnSceneSerialized(Scene& scene) override {
      scene.writeComponent("physics_body", {
        {"boundsCenterX",std::to_string(m_BoundsCenter.x)},
        {"boundsCenterY",std::to_string(m_BoundsCenter.y)},
        {"boundsCenterZ",std::to_string(m_BoundsCenter.z)},
        {"boundsX",std::to_string(m_Bounds.x)},
        {"boundsY",std::to_string(m_Bounds.y)},
        {"boundsZ",std::to_string(m_Bounds.z)},
        {"bodyType",std::to_string((int)m_BodyType)},
        {"angle", std::to_string(m_Angle)}
      });
    }

    void OnSceneDeserialized(Scene& scene) override {

    }

private:
    void transformEventHandler(const Vector3 &p, const Vector3 &r, const Vector3 &s) {
//        if(!WorldSimulation::getInstance().m_World->IsLocked())
        m_Body->SetTransform(b2Vec2(p.x, p.y), r.z * (float) (M_PI / 180));
    }

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

            ImGui::Checkbox("Allow Sleep", &m_AllowSleep);
//            if (s) {
            if (m_AllowSleep) {
                m_Body->SetAwake(false);
            } else {
                m_Body->SetAwake(true);
            }
//            }

            static Vector3 lastBounds{m_Bounds.x, m_Bounds.y, m_Bounds.z};
            static Vector3 lastBoundsCenter{m_BoundsCenter.x, m_BoundsCenter.y, m_BoundsCenter.z};

            bool cx = ImGui::DragFloat("Center X", &m_BoundsCenter.x, 0.01f);
            bool cy = ImGui::DragFloat("Center Y", &m_BoundsCenter.y, 0.01f);

            bool bx = ImGui::DragFloat("Bounding Box X", &m_Bounds.x, 0.01f, 0.01f, 100.0f);
            bool by = ImGui::DragFloat("Bounding Box Y", &m_Bounds.y, 0.01f, 0.01f, 100.0f);

            if (bx || by || cx || cy) {
                static b2Vec2 vertices[4];

                // Preventing assertion of the Box2D
                if (m_Bounds.x <= 0.05f) m_Bounds.x = 0.05f;
                if (m_Bounds.y <= 0.05f) m_Bounds.y = 0.05f;

                vertices[0] = b2Vec2(-m_Bounds.x + m_BoundsCenter.x, -m_Bounds.y + m_BoundsCenter.y);
                vertices[1] = b2Vec2(m_Bounds.x + m_BoundsCenter.x, -m_Bounds.y + m_BoundsCenter.y);
                vertices[2] = b2Vec2(m_Bounds.x + m_BoundsCenter.x, m_Bounds.y + m_BoundsCenter.y);
                vertices[3] = b2Vec2(-m_Bounds.x + m_BoundsCenter.x, m_Bounds.y + m_BoundsCenter.y);

//                m_Shape.SetAsBox(m_Bounds.x, m_Bounds.y);

                // there is only one fixture for the body.
                m_Body->DestroyFixture(m_Body->GetFixtureList());

                m_Shape.Set(&vertices[0], 4);

                m_Body->CreateFixture(&m_Shape, 1.0f);

                if (cx || cy) {
                    lastBoundsCenter.x = m_BoundsCenter.x;
                    lastBoundsCenter.y = m_BoundsCenter.y;
                }

                if (bx || by) {
                    lastBounds.x = m_Bounds.x;
                    lastBounds.y = m_Bounds.y;
                }
            }

            ImGui::Text("Awake: %s", m_Body->IsAwake() ? "true" : "false");
//            ImGui::Text("bd.fixedRotation = bool(%d);\n", m_flags & e_fixedRotationFlag);

            auto grav = m_Body->GetGravityScale();
            ImGui::Text("Gravity Scale: %.9g", grav);

            ImGui::Separator();
            ImGui::TreePop();
        }
    }

    Vector3 m_BoundsCenter{0.0f, 0.0f, 0.0f};
    Vector3 m_Bounds{0.0f, 0.0f, 0.0f};

    Vector3 m_Position{0.0f, 0.0f, 0.0f};
    bool m_AllowSleep{false};
    float m_Angle;
    BodyType m_BodyType;

    b2Body *m_Body;
    b2BodyDef m_BodyDef;
    b2FixtureDef m_Fixture;
    b2PolygonShape m_Shape;
};

#endif //ALIEN3D_PHYSICSBODY_H
