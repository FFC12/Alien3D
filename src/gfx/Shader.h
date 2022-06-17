#ifndef ALIEN3D_SHADER_H
#define ALIEN3D_SHADER_H

#include <Base.hpp>
#include <gfx/GfxBase.h>
#include <engine/Component.h>
#include <string>

class Shader : public Component {
public:
    Shader() = default;

    Shader(const std::string &vertSrc, const std::string &fragSrc) {
        createShader(vertSrc, fragSrc);
    }

    Shader(const char *vertSrc, const char *fragSrc) {

    }

    void createShader(const std::string &vertSrc, const std::string &fragSrc) {
        // Creating vertex and fragment shaders
        this->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        this->m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

        i32 succes;

        //TODO: Reset log after each success checking.
        char log[1024];

        const Gfx_char *fragShaderSrc = fragSrc.c_str();
        glShaderSource(this->m_FragmentShader, 1, &fragShaderSrc, nullptr);
        glCompileShader(this->m_FragmentShader);
        glGetShaderiv(this->m_FragmentShader, GL_COMPILE_STATUS, &succes);
        if (!succes) {
            glGetShaderInfoLog(this->m_FragmentShader, 1024, nullptr, log);
            ALIEN_ERROR("Shader Error::Fragment " << log);
        }

        const Gfx_char *vertShaderSrc = vertSrc.c_str();
        glShaderSource(this->m_VertexShader, 1, &vertShaderSrc, nullptr);
        glCompileShader(this->m_VertexShader);
        glGetShaderiv(this->m_VertexShader, GL_COMPILE_STATUS, &succes);
        if (!succes) {
            glGetShaderInfoLog(this->m_VertexShader, 1024, nullptr, log);
            ALIEN_ERROR("Shader Error::Vertex " << log);
        }

        // Compiling shaders and creating program
        this->m_Program = glCreateProgram();
        glAttachShader(this->m_Program, this->m_VertexShader);
        glAttachShader(this->m_Program, this->m_FragmentShader);
        glLinkProgram(this->m_Program);

        glGetProgramiv(this->m_Program, GL_LINK_STATUS, &succes);
        if (!succes) {
            glGetProgramInfoLog(this->m_Program, 1024, nullptr, log);
            ALIEN_ERROR("Shader Error::Program " << log);
        }
    }


    ~Shader() {
        glDeleteShader(this->m_FragmentShader);
        glDeleteShader(this->m_VertexShader);
    }

    Gfx_u32 getProgram() const {
        return this->m_Program;
    }

    void setUniform4(const std::string &uName, const glm::mat4 &val) {
        auto uPos = glGetUniformLocation(m_Program, uName.c_str());
        glCheckError();

        glUniformMatrix4fv(uPos, 1, GL_FALSE, glm::value_ptr(val));
        glCheckError();
    }

private:
    Gfx_u32 m_FragmentShader;
    Gfx_u32 m_VertexShader;
    Gfx_u32 m_Program;
};

#endif //ALIEN3D_SHADER_H
