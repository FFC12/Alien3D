#ifndef ALIEN3D_SHADER_MANAGER_H
#define ALIEN3D_SHADER_MANAGER_H

#include <unordered_map>
#include <optional>
#include <Base.hpp>
#include <gfx/GfxBase.h>
#include <utils/File.h>
#include <gfx/Shader.h>
#include <utils/ShaderParser.h>


/*
    -> Shader List
    -> Mustache Impl {{ }}
    -> Basic Lighting..

 */
class ShaderManager {
public:
    static ShaderManager &getInstance() {
        static ShaderManager ins;
        return ins;
    }

    std::optional<Gfx_u32> getShaderProgram(const std::string &shaderName) {
        if (ShaderList.count(shaderName) > 0) {
            auto entry = ShaderList.find(shaderName);
            return entry->second;
        } else {
            return std::nullopt;
        }
    }

    void
    addIncludeFile(const std::string &fileName, const std::string &path) {
        if (IncludeFileList.count(fileName) > 0) {
            return;
        } else {
            IncludeFileList[fileName] = std::move(Utils::ReadFile(path));
        }
    }

    void
    addIncludeDir(const std::string &fileName, const std::string &path) {
        // TODO: will be added
    }

    Shader
    addShader(const std::string &shaderName, const std::string &vertShaderPath, const std::string &fragShaderPath) {
        std::ifstream vertShaderFile, fragShaderFile;
        std::string line, vertData, fragData;

        vertShaderFile.open(vertShaderPath.c_str());
        if (vertShaderFile.is_open()) {
            while (std::getline(vertShaderFile, line)) {
                ShaderParser::ParseMustache(line, "common.glsl.h", IncludeFileList["common.glsl.h"]);
                vertData += (line + '\n');
            }
        }

        fragShaderFile.open(fragShaderPath.c_str());
        if (fragShaderFile.is_open()) {
            while (std::getline(fragShaderFile, line)) {
                ShaderParser::ParseMustache(line, "common.glsl.h", IncludeFileList["common.glsl.h"]);
                fragData += (line + '\n');
            }
        }

        Shader shader;
        shader.createShader(vertData, fragData);
        ShaderList[shaderName] = shader.getProgram();

        return shader;
    }

private:
    ShaderManager() = default;

    // <Name, Program ID>
    static inline std::unordered_map<std::string, Gfx_u32> ShaderList{};

    // <Mustache Name, Glsl Header File>
    static inline std::unordered_map<std::string, std::string> IncludeFileList{};
};

#endif