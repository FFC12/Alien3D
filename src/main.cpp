#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Application.h>
#include <engine/GameObject.h>
#include <engine/RenderQueue.h>
#include <engine/AssetImporter.h>

int main() {
    AlienApplication application(GfxDeviceType::GFX_OGL);

    ALIEN_INFO("Creating application...");
    if (!application.create(1920, 1080, "Alien3d")) {
        ALIEN_ASSERT2("FAILED");
    }

    std::shared_ptr<GameObject> room, room2;
    RenderQueue renderer;

    auto onInit = [&]() {
        auto model = AssetImporter::getInstance().loadModelFromPath("../res/scene.gltf");
        room = std::make_shared<GameObject>("test", model);
        room2 = std::make_shared<GameObject>("test2", model);

        renderer.addQueue(room);
        renderer.addQueue(room2);
    };

    auto onUpdate = [&]() {
        renderer.render();
    };

    application.start(onInit, onUpdate);

    return 0;
}
