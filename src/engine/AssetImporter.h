#ifndef ALIEN3D_ASSETIMPORTER_H
#define ALIEN3D_ASSETIMPORTER_H

#include <memory>
#include <Base.hpp>
#include <gfx/Buffer.h>
#include <engine/Model.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*
    -> Model model = loadModelFromPath();
    -> Model { Mesh, Texture }
    -> GameObject ( model );
*/

class
AssetImporter {
public:
    static AssetImporter &getInstance() {
        static AssetImporter ins;
        return ins;
    }

    std::shared_ptr<Model> loadModelFromPath(const std::string &path) {
        std::shared_ptr<Model> model = std::make_shared<Model>(path);

        //TODO: other flags will be added later
        auto *scene = m_Importer.ReadFile(path,
                                          aiProcess_Triangulate | aiProcess_GenNormals);

        // Asserts are only for debugging purpose
        // and will be converted to usual error message(s).
        if (scene == nullptr) {
            ALIEN_ASSERT2(m_Importer.GetErrorString());
        } else {
            if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                ALIEN_ASSERT2("Asset loading error!");
            }
        }

        loadSubNodes(scene->mRootNode, scene, model->m_Meshes);

        //TODO: this is not really effective thou
        model->m_HasMeshData = true;

        return std::move(model);
    }

    //delete copy/move constructors and operators
    //since this is a singleton object
    AssetImporter(const AssetImporter &) = delete;

    AssetImporter(AssetImporter &&) = delete;

    AssetImporter &operator=(const AssetImporter &) = delete;

    AssetImporter &operator=(const AssetImporter &&) = delete;

private:
    AssetImporter() = default;

    void loadSubNodes(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Mesh>> &meshes) {
        if (node->mNumMeshes > 0) {
            for (u32 i = 0; i < node->mNumMeshes; i++) {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

                auto meshData = buildAlienMeshData(mesh, scene);
                meshes.emplace_back(meshData);
            }
        }

        if (node->mNumChildren > 0) {
            for (u32 i = 0; i < node->mNumChildren; i++) {
                loadSubNodes(node->mChildren[i], scene, meshes);
            }
        }
    }

    static std::shared_ptr<Mesh>
    buildAlienMeshData(aiMesh *mesh, const aiScene *scene) {
        std::shared_ptr<Mesh> alienMeshData;
        std::vector<f32> vertices, texCoords, tangents, bitangents, normals;
        std::vector<u32> indices;


        for (u32 i = 0; i < mesh->mNumVertices; i++) {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            if (mesh->HasNormals()) {
                normals.push_back(mesh->mNormals[i].x);
                normals.push_back(mesh->mNormals[i].y);
                normals.push_back(mesh->mNormals[i].z);
            }

            if (mesh->HasTextureCoords(0)) {
                texCoords.push_back(mesh->mTextureCoords[0][i].x);
                texCoords.push_back(mesh->mTextureCoords[0][i].y);

                if (mesh->mTangents) {
                    tangents.push_back(mesh->mTangents[i].x);
                    tangents.push_back(mesh->mTangents[i].y);
                }

                if (mesh->mBitangents) {
                    bitangents.push_back(mesh->mBitangents[i].x);
                    bitangents.push_back(mesh->mBitangents[i].y);
                }
            } else {
                texCoords.push_back(0);
                texCoords.push_back(0);
            }
        }


        // retrieve the indices
        if (mesh->HasFaces()) {
            for (u32 i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for (u32 j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j]);
                }
            }
        }

        // set-up vertex data (vertex descriptor)
        VertexDescriptor<f32> vertexDescriptor;
        vertexDescriptor.vertices = std::move(vertices);
        vertexDescriptor.normals = std::move(normals);
        vertexDescriptor.tangents = std::move(tangents);
        vertexDescriptor.bitangents = std::move(bitangents);
        vertexDescriptor.textureCoords = std::move(texCoords);
        vertexDescriptor.indices = std::move(indices);
        vertexDescriptor.setHasIndices(mesh->HasFaces());

        alienMeshData = std::make_shared<Mesh>(vertexDescriptor);

        auto *mat = scene->mMaterials[mesh->mMaterialIndex];
        processTextures(mat, aiTextureType_DIFFUSE, "_Diffuse");

        return std::move(alienMeshData);
    }

    static void
    processTextures(aiMaterial *mat, aiTextureType type, const std::string &texName) {
        //TODO: Needs some improvements (see. Texture.h/Texture.cpp)
    }

    // WARN: Not thread-safe class
    Assimp::Importer m_Importer;
};


#endif //ALIEN3D_ASSETIMPORTER_H
