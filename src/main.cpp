#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "engine/Light.h"

#include <Application.h>
#include <engine/GameObject.h>
#include <engine/Sprite.h>
#include <engine/SpriteAnimation.h>
#include <engine/RenderQueue.h>
#include <engine/AssetImporter.h>
#include <api/LuaBridge.hpp>
#include <engine/Physics2D.h>
#include <engine/PhysicsBody.h>

int main() {
    AlienApplication application(GfxDeviceType::GFX_OGL);

    ALIEN_INFO("Creating application...");
    if (!application.create(1920, 1080, "Alien3d")) {
        ALIEN_ASSERT2("FAILED");
    }

    RenderQueue renderer;
    std::shared_ptr<Sprite> explosion;
    std::shared_ptr<Sprite> character;
    std::shared_ptr<SpriteAnimation> explosionAnim, charAnim;

    auto onInit = [&]() {
        explosion = std::make_shared<Sprite>("explosion", RESOURCE_PATH("assets/explosion_3.png"), false, true);
        explosionAnim = std::make_shared<SpriteAnimation>(explosion, 682, 682);
        explosion->attachComponent(explosionAnim, "sprite_animation");
        renderer.addQueue(explosion);

        character = std::make_shared<Sprite>("character", RESOURCE_PATH("assets/1/Walk_attack.png"), false, true);
        charAnim = std::make_shared<SpriteAnimation>(character, 48, 48);
        auto transform = character->getComponent<Transform>("transform");
        transform->setScale(Vector3(0.1f, 0.1f, 0.1f));
        character->attachComponent(charAnim, "sprite_animation");
        renderer.addQueue(character);
    };

    auto onUpdate = [&]() {
        renderer.render();
    };

    application.start(onInit, onUpdate, [&]() {}, false);
}


//int main() {
//    AlienApplication application(GfxDeviceType::GFX_OGL);
//
//    ALIEN_INFO("Creating application...");
//    if (!application.create(1920, 1080, "Alien3d")) {
//        ALIEN_ASSERT2("FAILED");
//    }
//
//    auto batchSize = SpriteBatcher::GetBatchSize();
//    ALIEN_INFO("Batch Size: " << batchSize);
//
//    std::shared_ptr<SpriteBatcher> spriteBatcher[1];
//    std::shared_ptr<Sprite> sprite[3];
//    RenderQueue renderer;
//    auto world = WorldSimulation::getInstance();
//
//    auto onInit = [&]() {
//        spriteBatcher[0] = std::make_shared<SpriteBatcher>();
//
//        //-- Character
//        sprite[0] = std::make_shared<Sprite>("test" + std::to_string(0), RESOURCE_PATH("sprite.png"), true, true);
//        spriteBatcher[0]->add(sprite[0]);
//        auto component = sprite[0]->getComponent<Transform>();
//        component->setPosition(Vector3(0.0f, 20.0f, 0.0f));
//        component->setRotation(Vector3(0.0f, 0.0f, 0.0f));
//        component->setScale(Vector3(1.0f, 1.0f, 1.0f));
//        auto characterBody = std::make_shared<PhysicsBody>(sprite[0], BodyType::Dynamic,
//                                                           PhysicalMaterial{.friction=0.01f, .density=1.0f});
//        sprite[0]->attachComponent(characterBody);
//
//        //-- Ground
//        sprite[1] = std::make_shared<Sprite>("test" + std::to_string(1), RESOURCE_PATH("ground.png"), true, true);
//        spriteBatcher[0]->add(sprite[1]);
//        component = sprite[1]->getComponent<Transform>();
//        component->setPosition(Vector3(0.0f, -4.0f, 0.0f));
//        component->setScale(Vector3(100.0, 20.0f, 1.0f));
//        auto groundBody = std::make_shared<PhysicsBody>(sprite[1], BodyType::Static);
//        sprite[1]->attachComponent(groundBody);
//
//        spriteBatcher[0]->batch();
//        renderer.addQueue(spriteBatcher[0]);
//
////        sprite[2] = std::make_shared<Sprite>("test" + std::to_string(2), RESOURCE_PATH("sprite.png"), true, true);
////        spriteBatcher->add(sprite[2]);
////
////        sprite[3] = std::make_shared<Sprite>("test" + std::to_string(3), RESOURCE_PATH("logo.png"), true, true);
////        spriteBatcher->add(sprite[3]);
//
//    };
//
//    auto onUpdate = [&]() {
//        renderer.render();
//        world.step();
//    };
//
//    auto onKeyboardHandling = [&]() {
//        if (glfwGetKey(application.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//            glfwSetWindowShouldClose(application.getWindow(), true);
//        }
//
//        auto component = sprite[0]->getComponent<PhysicsBody>();
//        auto velocity = component->getLinearVelocity();
//
//        bool allowed = false;
//        if ((velocity.x < 0.1f && velocity.x > -0.1f) && (velocity.y < 0.1f && velocity.y > -0.1f)) {
//            allowed = true;
//        }
//
//        if (glfwGetKey(application.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
//            component->applyForceImpulsive(Vector3(0.0f, 1.0f, 0.0f), 100 * AlienApplication::DeltaTime);
//
////        if (glfwGetKey(application.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
////            component->applyForceImpulsive(Vector3(-4.0f, 5.0f, 0.0f), 1);
//
//        if (glfwGetKey(application.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
//            if (allowed) {
//                component->applyForce(Vector3(-1.0f, 0.0f, 0.0f), 200);
//                auto transform = sprite[0]->getComponent<Transform>();
//                transform->setScale(Vector3(-1.0f, 1.0f, 1.0f));
//            }
//        }
//
//        if (glfwGetKey(application.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
//            if (allowed) {
//                component->applyForce(Vector3(1.0f, 0.0f, 0.0f), 200);
//                auto transform = sprite[0]->getComponent<Transform>();
//                transform->setScale(Vector3(1.0f, 1.0f, 1.0f));
//            }
//        }
//    };
//
//    application.start(onInit, onUpdate, onKeyboardHandling, true);
//
//    return 0;
//}

//int main() {
//    AlienApplication application(GfxDeviceType::GFX_OGL);
//
//    ALIEN_INFO("Creating application...");
//    if (!application.create(1920, 1080, "Alien3d")) {
//        ALIEN_ASSERT2("FAILED");
//    }
//
//    std::shared_ptr<GameObject> scene;
//    std::shared_ptr<Sprite> logo;
//    std::shared_ptr<Light> light;
//    RenderQueue renderer;
//
//    auto onInit = [&]() {
//        auto model = AssetImporter::getInstance().loadModelFromPath("../res/scene.gltf");
//        scene = std::make_shared<GameObject>("room", model);
//
//        light = std::make_shared<Light>(Vector3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), LightType::PointLight,
//                                        1.0f);
//        renderer.addQueue(scene);
//    };
//
//    auto onUpdate = [&]() {
//        renderer.render();
//    };
//
//    application.
//            start(onInit, onUpdate
//    );
//
//    return 0;
//}