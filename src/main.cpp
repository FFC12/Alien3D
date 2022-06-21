#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Application.h>
#include <engine/GameObject.h>
#include <engine/RenderQueue.h>

// Model Matrix (for each gameobject & identity matrix)
// View and Projection Matrices (world camera)

// Create Gameobject [Components] [drawCall] -> Adding to Render Queue
// -> Query Queue -> Draw Call for each GameObject
int main() {
    AlienApplication application(GfxDeviceType::GFX_OGL);

    ALIEN_INFO("Creating application...");
    if (!application.create(1920, 1080, "Alien3d")) {
        ALIEN_ASSERT2("FAILED");
        return 1;
    }

    std::shared_ptr<GameObject> cube;
    RenderQueue renderer;

    auto onInit = [&]() {
        cube = std::make_shared<GameObject>("test", Primitive::CUBE);
        renderer.addQueue(cube);
    };

    auto onUpdate = [&]() {
        renderer.render();
    };

    application.start(onInit, onUpdate);

    return 0;
}
