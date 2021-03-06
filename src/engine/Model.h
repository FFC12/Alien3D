#ifndef ALIEN3D_MODEL_H
#define ALIEN3D_MODEL_H

#include <memory>
#include <Base.hpp>
#include <gfx/GfxBase.h>
#include <engine/Mesh.h>
#include <vector>

class AssetImporter;

class GameObject;

class Sprite;

class SpriteBatcher;

class Model {
    friend class AssetImporter;

    friend class GameObject;

    friend class Sprite;

    friend class SpriteBatcher;
public:
    Model(const std::string &path) : m_ModelPath(path) {

    }

    void render() {
        //GL
        renderGL();
    }

private:
    void initTextures(Gfx_u32 shaderProg) {
        for (auto &mesh: m_Meshes) {
            for (auto &texture: mesh->m_TexturePaths) {
                mesh->m_Texture.generateTexture(texture.second.c_str(), shaderProg, texture.first.c_str());
            }
        }
    }


    void renderGL() {
        for (auto &mesh: m_Meshes) {
            if (!mesh->m_Buffer.m_VertDesc.vertices.empty() && mesh->m_Buffer.m_BufferInitialized) {
                mesh->m_Texture.enableTexture();
                mesh->m_Buffer.enableVAO();
                if (mesh->m_Buffer.m_VertDesc.hasIndices()) {
                    glDrawElements(GL_TRIANGLES, mesh->m_Buffer.m_VertDesc.indices.size(), GL_UNSIGNED_INT, nullptr);
                    glCheckError();
                } else {
                    glDrawArrays(GL_TRIANGLES, 0, mesh->m_Buffer.m_VertDesc.vertices.size() *
                                                  mesh->m_Buffer.m_VertDesc.vertexCountPerIndex);
                    glCheckError();
                }
            }
        }
    }

    std::vector<std::shared_ptr<Mesh>> m_Meshes;
    bool m_HasMeshData{false};

    std::string m_ModelPath;
};

#endif //ALIEN3D_MODEL_H

