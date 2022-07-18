#ifndef ALIEN3D_GFXBASE_H
#define ALIEN3D_GFXBASE_H

#include <Base.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>

typedef GLenum Gfx_enum;
typedef GLint Gfx_i32;
typedef GLuint Gfx_u32;
typedef GLuint64 Gfx_u64;
typedef GLfloat Gfx_f32;
typedef GLdouble Gfx_f64;
typedef GLsizei Gfx_sizei;
typedef GLchar Gfx_char;
typedef GLboolean Gfx_bool;
typedef GLvoid Gfx_void;

enum GfxDeviceType {
    GFX_OGL,
    GFX_OGLES,
    GFX_VK,
    GFX_VKES,
    GFX_DX
};

class GfxBase {
public:
    GfxBase() = default;

    static i32 CreateGfxDevice(const GfxDeviceType &gfxDeviceType) {
        SelectedGfxDeviceType = gfxDeviceType;
        switch (gfxDeviceType) {
            case GFX_OGL: {
                if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                    std::cout << "FAILED" << std::endl;
                    return 1;
                }
                break;
            }
            case GFX_OGLES:
                break;
            case GFX_VK:
                break;
            case GFX_VKES:
                break;
            case GFX_DX:
                break;
            default:
                return 0;
        }
    }

    static inline GfxDeviceType SelectedGfxDeviceType{};

};

static Gfx_enum glCheckError_(const char *file, int line) {
    Gfx_enum errCode;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }

        ALIEN_ERROR(error + "|" + std::string(file) + "(" + std::to_string(line) + ")");
    }

    return errCode;
}

#define _GL_CHECK
#ifndef glCheckError
#ifdef _GL_CHECK
#define glCheckError() glCheckError_(__FILE__,__LINE__)
#else
#define do {} while(0);
#endif
#endif

#endif //ALIEN3D_GFXBASE_H
