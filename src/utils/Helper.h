#ifndef ALIEN3D_HELPER_H
#define ALIEN3D_HELPER_H

#include <Base.hpp>
#include <cmath>

namespace Utils {
    // Check if the both of float numbers are same?
    static inline bool CheckFloat(f32 a, f32 b) {
        return fabs(a - b) < FLT_EPSILON;
    }
}

#endif