#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <api/LuaBridge.hpp>
#include <engine/Component.h>
#include <engine/GameObject.h>
#include <engine/RenderQueue.h>
#include <engine/Sprite.h>

class Scripting : public Component {
public:
    Scripting() = default;

    static void init() {
        LuaObject &luaObject = LuaObject::getInstance();

//        luaObject.registerClassToAPI<GameObject>("GameObject", sol::constructors<GameObject(const std::string &)>());

        // -- Sprite
        luaObject.registerClassToAPI<Sprite>("Sprite", sol::constructors<
                Sprite(const std::string &, bool),
                Sprite(const std::string &, const std::string &, bool, bool)>());

        // -- SpriteBatcher
        luaObject.registerClassToAPI<SpriteBatcher>("SpriteBatcher", sol::constructors<SpriteBatcher>());


        // -- Transform

        // -- PhysicsBody

    }

private:
    void scriptingWidget() {

    }
};

#endif