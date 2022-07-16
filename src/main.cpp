#include <iostream>
#include <memory>

#include <Application.h>
#include <engine/Scripting.h>

int main() {
    AlienApplication application(GfxDeviceType::GFX_OGL);

    ALIEN_INFO("Creating application...");
    if (!application.create(1920, 1080, "Alien3d")) {
        ALIEN_ASSERT2("FAILED");
    }

    Scripting script;
    auto onInit = [&]() {
        script.loadScriptFromFile(RESOURCE_PATH("scripts/script.py"));
        Scripting::init();
    };

    auto onUpdate = [&]() {
        Scripting::run();
    };

    application.start(onInit, onUpdate, [&]() {}, false);

    return 0;
}