#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "engine/Light.h"

#include <Application.h>
#include <engine/AssetImporter.h>
#include <engine/Scripting.h>
#include <api/PythonBridge.h>
#include <engine/Physics2D.h>

int main() {
    AlienApplication application(GfxDeviceType::GFX_OGL);

    ALIEN_INFO("Creating application...");
    if (!application.create(1920, 1080, "Alien3d")) {
        ALIEN_ASSERT2("FAILED");
    }

    PythonBridge pythonBridge;
    WorldSimulation simulation = WorldSimulation::getInstance();
//    Sprite sprite("test", "../res/sprite.png", true);
//    PhysicsBody physicsBody(sprite, BodyType::Dynamic);

    auto onInit = [&]() {
        Scripting::init();
        pythonBridge.loadPythonScriptFromFile(RESOURCE_PATH("scripts/script.py"));
//        sprite.attachComponent(&physicsBody, "physics_body");
    };

    auto onUpdate = [&]() {
        simulation.step();
    };

    application.start(onInit, onUpdate, [&]() {}, false);

    return 0;
}