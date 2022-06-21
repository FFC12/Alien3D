#ifndef ALIEN3D_VECTOR_H
#define ALIEN3D_VECTOR_H
#include <Base.hpp>
#include <glm.hpp>


// TODO: needs some improvements...
struct Vector3 {
    Vector3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    Vector3(f32 s) : x(s), y(s), z(s) {}

    Vector3& operator=(Vector3& other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;

        return *this;
    }

    inline glm::vec3 operator=(const Vector3& v) {
        return { v.x, v.y, v.z};
    }

    inline Vector3 operator=(const glm::vec3& v) {
        return { v.x, v.y, v.z};
    }

    inline Vector3 operator+(const Vector3& v) {
        return { this->x + v.x, this->y + v.y, this->z + v.z};
    }

    inline Vector3 operator-(const Vector3& v) {
        return { this->x - v.x, this->y - v.y, this->z - v.z};
    }

    f32 x,y,z;
};


#endif