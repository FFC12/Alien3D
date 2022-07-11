#ifndef ALIEN3D_LUA_BRIDGE_HPP
#define ALIEN3D_LUA_BRIDGE_HPP

// This is an awkward problem...
#include "../../lib/sol3/sol/sol.hpp"

class LuaObject {
public:
    LuaObject() {
        m_Lua.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io);
    }

    void runScript(const std::string &script) {
        try {
            m_Lua.script(script);
        } catch (sol::error &err) {
            std::cerr << "Script compilation has failed:\n" << err.what() << std::endl;
        };
    }

private:
    sol::state m_Lua;
};

#endif