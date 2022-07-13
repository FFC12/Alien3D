#include "PythonBridge.h"
#include <engine/Sprite.h>
#include <engine/PhysicsBody.h>
#include <engine/Transform.h>
#include <engine/Vector.h>

PYBIND11_EMBEDDED_MODULE(alien3d, m) {
    py::class_<Sprite>(m, "Sprite")
            .def(py::init<const std::string &, const std::string &, bool>())
            .def(py::init<const std::string &, bool>())
            .def("attachComponent", &Sprite::attachComponent<PhysicsBody>)
//            .def("getComponent", &Sprite::getComponent<PhysicsBody>)
            .def("getComponent", &Sprite::getComponent<Transform>);

    py::enum_<BodyType>(m, "BodyType")
            .value("STATIC", BodyType::Static)
            .value("DYNAMIC", BodyType::Dynamic)
            .value("KINEMATIC", BodyType::Kinematic)
            .export_values();

    py::class_<PhysicsBody>(m, "PhysicsBody")
            .def(py::init<Sprite &, const BodyType &>());

    py::class_<Vector3>(m, "Vec3")
            .def(py::init<>())
            .def(py::init<f32, f32, f32>())
            .def(py::init<f32>());

    py::class_<Transform>(m, "Transform")
            .def(py::init<>())
            .def(py::init<const Vector3 &, const Vector3 &, const Vector3 &>())
            .def("setPosition", py::overload_cast<const Vector3 &>(&Transform::setPosition))
            .def("setPosition", py::overload_cast<f32, f32>(&Transform::setPosition));
}