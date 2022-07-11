#ifndef ALIEN3D_MESH_H
#define ALIEN3D_MESH_H

#include <memory>
#include <Base.hpp>
#include <gfx/GfxBase.h>
#include <gfx/Buffer.h>

class GameObject;

class Model;

class AssetImporter;

class Sprite;

class SpriteBatcher;

class Mesh {
    friend class GameObject;

    friend class Model;

    friend class AssetImporter;

    friend class Sprite;

    friend class SpriteBatcher;

public:
    Mesh() = default;
    Mesh(VertexDescriptor<f32> vertexDescriptor) {
        m_Buffer.m_VertDesc = std::move(vertexDescriptor);
    }

private:
    Buffer<f32> m_Buffer;
    Texture m_Texture;

    // name - path
    std::unordered_map<std::string, std::string> m_TexturePaths;
};

#endif //ALIEN3D_MESH_H
