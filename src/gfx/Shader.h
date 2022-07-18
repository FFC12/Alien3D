#ifndef ALIEN3D_SHADER_H
#define ALIEN3D_SHADER_H

#include <Base.hpp>
#include <gfx/GfxBase.h>
#include <engine/Component.h>
#include <string>
#include "imgui.h"


class Shader : public Component {
public:
    Shader() {
        componentType = SHADER;
    }

    Shader(const Shader &other) {
        this->m_FragmentShader = other.m_FragmentShader;
        this->m_VertexShader = other.m_VertexShader;
        this->m_Program = other.m_Program;
        this->m_FragmentSource = other.m_FragmentSource;
        this->m_VertexSource = other.m_VertexSource;
        this->m_UUID = Utils::getUUID();
//        this->m_UUID = other.m_UUID;
    }

    Shader(const std::string &vertSrc, const std::string &fragSrc) {
        componentType = SHADER;
        m_VertexSource = vertSrc;
        m_FragmentSource = fragSrc;
        createShader(vertSrc, fragSrc);
    }

    Shader(const char *vertSrc, const char *fragSrc) {
        componentType = SHADER;
    }

    Shader operator=(const Shader& other) {
        this->m_FragmentShader = other.m_FragmentShader;
        this->m_VertexShader = other.m_VertexShader;
        this->m_Program = other.m_Program;
        this->m_FragmentSource = other.m_FragmentSource;
        this->m_VertexSource = other.m_VertexSource;
        this->m_UUID = Utils::getUUID();
//        this->m_UUID = other.m_UUID;
        return *this;
    }

    void createShader(const std::string &vertSrc, const std::string &fragSrc) {
        m_VertexSource = vertSrc;
        m_FragmentSource = fragSrc;

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

    void useProgram() {
        glUseProgram(this->m_Program);
        glCheckError();
    }

    void disableProgram() {
        glUseProgram(0);
        glCheckError();
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

    void setVector3(const std::string &uName, const glm::vec3 &val) {
        auto uPos = glGetUniformLocation(m_Program, uName.c_str());
        glCheckError();

        glUniform3fv(uPos, 1, glm::value_ptr(val));
        glCheckError();
    }

    void setVector2(const std::string &uName, const glm::vec2 &val) {
        auto uPos = glGetUniformLocation(m_Program, uName.c_str());
        glCheckError();

        glUniform2fv(uPos, 1, glm::value_ptr(val));
        glCheckError();
    }

    void setBool(const std::string &uName, const bool &val) {
        auto uPos = glGetUniformLocation(m_Program, uName.c_str());
        glCheckError();

        glUniform1i(uPos, val);
        glCheckError();
    }

    void setInt(const std::string &uName, const int &val) {
        auto uPos = glGetUniformLocation(m_Program, uName.c_str());
        glCheckError();

        glUniform1i(uPos, val);
        glCheckError();
    }

    void setIntArray(const std::string &uName, const std::vector<i32> &val) {
        auto uPos = glGetUniformLocation(m_Program, uName.c_str());
        glCheckError();

        glUniform1iv(uPos, val.size(), &val[0]);
        glCheckError();
    }

    void setFloat(const std::string &uName, const float &val) {
        auto uPos = glGetUniformLocation(m_Program, uName.c_str());
        glCheckError();

        glUniform1f(uPos, val);
        glCheckError();
    }

    void OnComponentWidgetDrawn() override {
        shaderWidget();
    }

private:
    void shaderWidget() {
        if (ImGui::TreeNode("Shader")) {
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;
//        ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
//        ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
//        ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", &flags,
//                             ImGuiInputTextFlags_CtrlEnterForNewLine);
            ImGui::InputTextMultiline("##source", const_cast<char *>(m_FragmentSource.c_str()), m_FragmentSource.size(),
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);

            ImGui::InputTextMultiline("##source2", const_cast<char *>(m_VertexSource.c_str()), m_VertexSource.size(),
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);

            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    std::string m_FragmentSource;
    std::string m_VertexSource;

    Gfx_u32 m_FragmentShader;
    Gfx_u32 m_VertexShader;
    Gfx_u32 m_Program;
};

#endif //ALIEN3D_SHADER_H
