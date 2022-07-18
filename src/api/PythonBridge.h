#ifndef ALIEN3D_PYTHONBRIDGE_H
#define ALIEN3D_PYTHONBRIDGE_H

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <utils/File.h>
#include <Base.hpp>
#include <map>

namespace py = pybind11;
using namespace py::literals;

class PythonBridge {
public:
    static PythonBridge &getInstance() {
        static PythonBridge ins;
        return ins;
    }

    void execInit(const py::object &scriptObject) {
        auto startMethod = scriptObject.attr("start");
        startMethod();
    }

    void execUpdate(const py::object &scriptObject) {
        auto updateMethod = scriptObject.attr("update");
        updateMethod();
    }

private:
    PythonBridge() {
        init();
    };

    void init() {
        auto alien3d = py::module_::import("alien3d");
        //#Sprite,PhysicsBody,BodyType,Transform
    }

    py::scoped_interpreter m_Interpreter{};
};

#endif //ALIEN3D_PYTHONBRIDGE_H
