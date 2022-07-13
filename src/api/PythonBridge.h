#ifndef ALIEN3D_PYTHONBRIDGE_H
#define ALIEN3D_PYTHONBRIDGE_H

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <utils/File.h>

namespace py = pybind11;
using namespace py::literals;

class PythonBridge {
public:
    PythonBridge() {
        init();
    };

    void loadPythonScriptFromFile(const std::string &path) {
        auto data = Utils::ReadFile(path);
        py::exec(data);
    }

private:
    void init() {
        auto alien3d = py::module_::import("alien3d");
        //#Sprite,PhysicsBody,BodyType,Transform
    }

    py::scoped_interpreter m_Interpreter{};
};

#endif //ALIEN3D_PYTHONBRIDGE_H
