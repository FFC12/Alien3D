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

    std::shared_ptr<GameObject> cube;
    RenderQueue renderer;

    auto onInit = [&]() {
        auto model = AssetImporter::getInstance().loadModelFromPath("../res/teapot.obj");
        cube = std::make_shared<GameObject>("test", model);
        renderer.addQueue(cube);
    };

    auto onUpdate = [&]() {
        renderer.render();
    };

    application.start(onInit, onUpdate);

    return 0;
}
