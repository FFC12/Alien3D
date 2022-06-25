#ifndef ALIEN3D_BUFFER_H
#define ALIEN3D_BUFFER_H

#include <vector>
#include <type_traits>
#include <Base.hpp>
#include <gfx/GfxBase.h>

/*   Vertex
    Descriptor
       |
  RAM -> Buffer -> GPU
*/

//TODO: Destructor for buffer objects (glDeleteBuffers)
template<typename T>
struct VertexDescriptor {
    std::vector<T> vertices;
    std::vector<T> normals;
    std::vector<T> textureCoords;
    std::vector<T> tangents;
    std::vector<T> bitangents;
    std::vector<Gfx_u32> indices;
    size_t vertexCountPerIndex;

    explicit VertexDescriptor(size_t vertexCount = 3) : vertexCountPerIndex(vertexCount) {

    }

    VertexDescriptor(const std::vector<T> &vertices,
                     const std::vector<T> &colors,
                     const std::vector<T> &normals,
                     const std::vector<T> &textureCoord,
                     const std::vector<T> &tangents,
                     const std::vector<T> &bitangents,
                     size_t vertexCount = 3)
            : vertices(vertices), normals(normals), tangents(tangents), bitangents(bitangents),
              textureCoords(textureCoord),
              vertexCountPerIndex(vertexCount) {}

    VertexDescriptor(const std::vector<T> &vertices,
                     const std::vector<T> &colors,
                     const std::vector<T> &normals,
                     const std::vector<T> &textureCoord,
                     const std::vector<T> &tangents,
                     const std::vector<T> &bitangents,
                     bool hasIndices,
                     const std::vector<Gfx_u32> &indices,
                     size_t vertexCount = 3)
            : vertices(vertices),
              normals(normals),
              textureCoords(textureCoord),
              tangents(tangents), bitangents(bitangents),
              m_hasIndices(hasIndices),
              indices(indices),
              vertexCountPerIndex(vertexCount) {}

    VertexDescriptor(const VertexDescriptor<T> &vertexDescriptor) {
        vertices = vertexDescriptor.vertices;
        normals = vertexDescriptor.normals;
        textureCoords = vertexDescriptor.textureCoords;
        tangents = vertexDescriptor.tangents;
        bitangents = vertexDescriptor.bitangents;
        indices = vertexDescriptor.indices;
        m_hasIndices = vertexDescriptor.hasIndices();
        vertexCountPerIndex = vertexDescriptor.vertexCountPerIndex;
    }

    VertexDescriptor<T> &operator=(const VertexDescriptor<T> &vertexDescriptor) {
        vertices = vertexDescriptor.vertices;
        normals = vertexDescriptor.normals;
        textureCoords = vertexDescriptor.textureCoords;
        tangents = vertexDescriptor.tangents;
        bitangents = vertexDescriptor.bitangents;
        indices = vertexDescriptor.indices;
        m_hasIndices = vertexDescriptor.hasIndices();
        vertexCountPerIndex = vertexDescriptor.vertexCountPerIndex;

        return *this;
    }

    VertexDescriptor(const VertexDescriptor<T> &&vertexDescriptor) {
        vertices = std::move(vertexDescriptor.vertices);
        normals = std::move(vertexDescriptor.normals);
        textureCoords = std::move(vertexDescriptor.textureCoords);
        tangents = std::move(vertexDescriptor.tangents);
        bitangents = std::move(vertexDescriptor.bitangents);
        indices = std::move(vertexDescriptor.indices);
        m_hasIndices = std::move(vertexDescriptor.hasIndices());
        vertexCountPerIndex = std::move(vertexDescriptor.vertexCountPerIndex);
    }

    VertexDescriptor<T> &operator=(const VertexDescriptor<T> &&vertexDescriptor) {
        vertices = std::move(vertexDescriptor.vertices);
        normals = std::move(vertexDescriptor.normals);
        textureCoords = std::move(vertexDescriptor.textureCoords);
        tangents = std::move(vertexDescriptor.tangents);
        bitangents = std::move(vertexDescriptor.bitangents);
        indices = std::move(vertexDescriptor.indices);
        m_hasIndices = std::move(vertexDescriptor.hasIndices());
        vertexCountPerIndex = std::move(vertexDescriptor.vertexCountPerIndex);

        return *this;
    }


    [[nodiscard]]
    bool hasIndices() const {
        return m_hasIndices;
    }

    void setHasIndices(bool v) {
        this->m_hasIndices = v;
    }

private:
    bool m_hasIndices{false};
};

enum BufferType {
    STATIC,
    DYNAMIC,
    STREAM
};

class Mesh;

class AssetImporter;

class Model;

template<typename T>
class Buffer {
    friend class Mesh;

    friend class Model;

    friend class AssetImporter;

public:
    Buffer() {}

    Buffer(const VertexDescriptor<T> &vertexDescriptor, BufferType bufferType)
            : m_VertDesc(vertexDescriptor) {
        ALIEN_ASSERT(vertexDescriptor.vertices.size() % vertexDescriptor.vertexCountPerIndex != 1 &&
                     "Vertices count must be divisable by vertex count per index!");

        this->m_BufferType = bufferType;
    }

    void initBuffer(BufferType bufferType) {
        ALIEN_ASSERT(m_VertDesc.vertices.size() % m_VertDesc.vertexCountPerIndex != 1 &&
                     "Vertices count must be divisable by vertex count per index!");

        Gfx_enum bufferDrawType;
        switch (bufferType) {
            case STATIC:
                bufferDrawType = GL_STATIC_DRAW;
                break;
            case DYNAMIC:
                bufferDrawType = GL_DYNAMIC_DRAW;
                break;
            case STREAM:
                bufferDrawType = GL_STREAM_DRAW;
                break;
        }

        switch (GfxBase::SelectedGfxDeviceType) {
            case GFX_OGL: {
                allocateGLBuffer(m_VertDesc, bufferDrawType);
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
        }

    }

    void initBuffer(const VertexDescriptor<T> &vertexDescriptor, BufferType bufferType) {
        ALIEN_ASSERT(vertexDescriptor.vertices.size() % vertexDescriptor.vertexCountPerIndex != 1 &&
                     "Vertices count must be divisable by vertex count per index!");

        Gfx_enum bufferDrawType;
        switch (bufferType) {
            case STATIC:
                bufferDrawType = GL_STATIC_DRAW;
                break;
            case DYNAMIC:
                bufferDrawType = GL_DYNAMIC_DRAW;
                break;
            case STREAM:
                bufferDrawType = GL_STREAM_DRAW;
                break;
        }

        switch (GfxBase::SelectedGfxDeviceType) {
            case GFX_OGL: {
                allocateGLBuffer(vertexDescriptor, bufferDrawType);
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
        }

    }

    Buffer &setProgram(Gfx_u32 program) {
        this->m_ShaderProgram = program;
        return *this;
    }

    Gfx_u32 getVAO() {
        return this->m_VAO;
    }

    void enableVAO() {
        glBindVertexArray(this->m_VAO);
    }

private:
    //TODO: IBO will be implemented for the indices..
    void allocateGLBuffer(const VertexDescriptor<T> &vertexDesc, Gfx_enum bufferType) {
        //3
        std::vector<T> payload(vertexDesc.vertices.begin(), vertexDesc.vertices.end());

        //3
        payload.insert(payload.end(), vertexDesc.normals.begin(), vertexDesc.normals.end());

        //2
        payload.insert(payload.end(), vertexDesc.textureCoords.begin(), vertexDesc.textureCoords.end());

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        if (vertexDesc.hasIndices())
            glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, payload.size() * sizeof(T), &payload[0], bufferType);
        glCheckError();

        if (vertexDesc.hasIndices()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexDesc.indices.size() * sizeof(Gfx_u32),
                         &vertexDesc.indices[0], bufferType);
            glCheckError();
        }

        /*
        auto posLoc = glGetAttribLocation(this->m_ShaderProgram, "position");
        auto colorLoc = glGetAttribLocation(this->m_ShaderProgram, "color");
        auto normalLoc = glGetAttribLocation(this->m_ShaderProgram, "normal");
        auto texCoordLoc = glGetAttribLocation(this->m_ShaderProgram, "texCoord");
         */
        glBindAttribLocation(this->m_ShaderProgram, 0, "position");
        glBindAttribLocation(this->m_ShaderProgram, 1, "normal");
        glBindAttribLocation(this->m_ShaderProgram, 2, "texCoord");

        Gfx_u32 posLoc = 0;
        Gfx_u32 normalLoc = 1;
        Gfx_u32 texCoordLoc = 2;

        size_t verticesSize = vertexDesc.vertices.size();
        size_t normalsSize = vertexDesc.normals.size();
//        size_t texCoordSize = vertexDesc.textureCoord.size();

        if (std::is_same<T, double>::value) {
            glVertexAttribPointer(posLoc, 3, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) 0);
            glEnableVertexAttribArray(posLoc);

            glVertexAttribPointer(normalLoc, 3, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) (verticesSize * sizeof(double)));
            glEnableVertexAttribArray(normalLoc);

            glVertexAttribPointer(texCoordLoc, 2, GL_DOUBLE, GL_FALSE, 0,
                                  (Gfx_void *) ((normalsSize + verticesSize) * sizeof(double)));
            glEnableVertexAttribArray(texCoordLoc);
        } else if (std::is_same<T, float>::value) {
            glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (Gfx_void *) 0);
            glEnableVertexAttribArray(posLoc);

            glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0,
                                  (Gfx_void *) ((verticesSize) * sizeof(float)));
            glEnableVertexAttribArray(normalLoc);

            glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0,
                                  (Gfx_void *) ((normalsSize + verticesSize) * sizeof(float)));
            glEnableVertexAttribArray(texCoordLoc);
        } else {
            static_assert("Only float and double type of vertex descriptor is allowed!");
        }

        m_BufferInitialized = true;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    VertexDescriptor<T> m_VertDesc;
    BufferType m_BufferType;

    bool m_BufferInitialized {false};

    Gfx_u32 m_VBO{};
    Gfx_u32 m_VAO{};
    Gfx_u32 m_EBO{};
    Gfx_u32 m_ShaderProgram{};
};


#endif //ALIEN3D_BUFFER_H
