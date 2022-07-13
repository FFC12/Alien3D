#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <engine/Component.h>
#include <engine/GameObject.h>
#include <engine/RenderQueue.h>
#include <engine/PhysicsBody.h>
#include <engine/Sprite.h>
#include <api/PythonBridge.h>

class Scripting : public Component {
public:
    Scripting() = default;

    static void init() {

    }

private:
    void scriptingWidget() {

    }
};

#endif