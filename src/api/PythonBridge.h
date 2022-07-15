#ifndef ALIEN3D_PYTHONBRIDGE_H
#define ALIEN3D_PYTHONBRIDGE_H

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <utils/File.h>
#include <Base.hpp>

namespace py = pybind11;
using namespace py::literals;

class PythonBridge {
public:
    static PythonBridge &getInstance() {
        static PythonBridge ins;
        return ins;
    }

    void loadPythonScriptFromFile(const std::string &path) {
        auto data = Utils::ReadFile(path);

        auto pos = path.find_last_of('/');
        auto scriptName = path.substr(pos + 1, path.size());

        if (m_Scripts.count(scriptName) > 0) {
            ALIEN_ERROR("There is already a script with this name '" + scriptName + "'. Change the name of script!");
        }

        m_Scripts[scriptName] = data;
//        py::exec(data);
    }

    void exec() {
        for (auto &script: m_Scripts) {
            ALIEN_INFO("Executing " + script.first + ".py");
            py::exec(script.second);
        }
    }

private:
    PythonBridge() {
        init();
    };

    void init() {
        auto alien3d = py::module_::import("alien3d");
        //#Sprite,PhysicsBody,BodyType,Transform
    }

    // script name - data
    std::unordered_map<std::string, std::string> m_Scripts;
    py::scoped_interpreter m_Interpreter{};
};

#endif //ALIEN3D_PYTHONBRIDGE_H
