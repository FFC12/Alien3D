#ifndef ALIEN3D_LUA_BRIDGE_HPP
#define ALIEN3D_LUA_BRIDGE_HPP

// This is an awkward problem...
#include "../../lib/sol3/sol/sol.hpp"

class LuaObject {
public:
    static LuaObject &getInstance() {
        static LuaObject ins;
        return ins;
    }

    void loadScriptFromFile(const std::string &path) {
        try {
            auto p = m_State.load_file(path);
            p();
        } catch (sol::error &err) {
            std::cerr << "Script compilation has failed:\n" << err.what() << std::endl;
        };
//        std::string data;
//
//        // If the script does not exist read from file otherwise get the
//        // data from hashmap.
//        if (m_Scripts.count(path) > 0) {
//            data = m_Scripts[path];
//        } else {
//            m_Scripts[path] = Utils::ReadFile(path);
//            data = m_Scripts[path];
//        }
//
//        loadScript(data);
    }

    void loadScript(const std::string &script) {
        try {
            auto lr = m_State.load(script);
            if (!lr.valid()) {
                sol::error err = lr;
                std::cerr << err.what() << std::endl;
            }
//            sol_c_assert(lr.valid());

//            auto target = lr.get<sol::protected_function>();
//            auto targetBc = target.dump();
//
//            m_Bytecodes.emplace_back(targetBc);
        } catch (sol::error &err) {
            std::cerr << "Script compilation has failed:\n" << err.what() << std::endl;
        };
    }

    template<typename Class, typename... Args>
    sol::usertype<Class> registerClassToAPI(Args &&...args) {
        return m_State.new_usertype<Class>(std::forward<Args>(args)...);
    }

    void onInit() {
        auto init = m_State["oninit"];
        init();
    }

    void onUpdate() {
        auto update = m_State["onupdate"];
        update();
    }

private:
    LuaObject() {
        m_State.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io);
        initModules();
    }

    void initModules() {
        std::string base = "../res/scripts/lib/";
        loadScriptFromFile(base + "alien2d.lua");
    }

    sol::state m_State;

    std::vector<sol::bytecode> m_Bytecodes;
    std::unordered_map<std::string, std::string> m_Scripts;
};

#endif