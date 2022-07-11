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

    std::vector<T> interleavedData;

    explicit VertexDescriptor(size_t vertexCount = 3) : vertexCountPerIndex(vertexCount) {

    }

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
        interleavedData = vertexDescriptor.interleavedData;
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
        interleavedData = vertexDescriptor.interleavedData;
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
        interleavedData = std::move(vertexDescriptor.interleavedData);
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
        interleavedData = std::move(vertexDescriptor.interleavedData);
        m_hasIndices = std::move(vertexDescriptor.hasIndices());
        vertexCountPerIndex = std::move(vertexDescriptor.vertexCountPerIndex);

        return *this;
    }

    // Remember, it's data gathering on the left side not right side (v0)
    static void BatchBuffer(VertexDescriptor<T> &v0, VertexDescriptor<T> &v1) {
        assert(v0.vertexCountPerIndex == v1.vertexCountPerIndex &&
               "Both vertices must have same vertex count per index!");
        v0.interleavedData.insert(v0.interleavedData.end(), v1.interleavedData.begin(), v1.interleavedData.end());
//        v0.vertices.insert(v0.vertices.end(), v1.vertices.begin(), v1.vertices.end());
        v0.indices.insert(v0.indices.end(), v1.indices.begin(), v1.indices.end());
//        v0.normals.insert(v0.normals.end(), v1.normals.begin(), v1.normals.end());
//        v0.textureCoords.insert(v0.textureCoords.end(), v1.textureCoords.begin(), v1.textureCoords.end());
//        v0.tangents.insert(v0.tangents.end(), v1.tangents.begin(), v1.tangents.end());
//        v0.bitangents.insert(v0.bitangents.end(), v1.bitangents.begin(), v1.bitangents.end());
    }

    [[nodiscard]]
    bool hasIndices() const {
        return m_hasIndices;
    }

    void setHasIndices(bool v) {
        this->m_hasIndices = v;
    }

    std::vector<T> &asInterleavedArray() {
        return interleavedData;
    }

private:
    bool m_hasIndices{false};
};

enum BufferType {
    STATIC = GL_STATIC_DRAW,
    DYNAMIC = GL_DYNAMIC_DRAW,
    STREAM = GL_STREAM_DRAW
};

class Mesh;

class AssetImporter;

class Model;

class SpriteBatcher;

template<typename T>
class Buffer {
    friend class Mesh;

    friend class Model;

    friend class AssetImporter;

    friend class SpriteBatcher;

public:
    Buffer() {}

    Buffer(const VertexDescriptor<T> &vertexDescriptor, BufferType bufferType)
            : m_VertDesc(vertexDescriptor) {
        ALIEN_ASSERT(m_VertDesc.vertices.size() % m_VertDesc.vertexCountPerIndex != 1 &&
                     "Vertices count must be divisable by vertex count per index!");

        this->m_BufferType = bufferType;
    }

    void initBuffer();

    void initBuffer(BufferType bufferType);

    void initBuffer(const VertexDescriptor<T> &vertexDescriptor, BufferType bufferType);

    Buffer &setProgram(Gfx_u32 program) {
        this->m_ShaderProgram = program;
        return *this;
    }

    Gfx_u32 getVAO() {
        return this->m_VAO;
    }

    Gfx_u32 getVBO() {
        return this->m_VBO;
    }

    Gfx_u32 getEBO() {
        return this->m_EBO;
    }

    void enableVAO() {
        glBindVertexArray(this->m_VAO);
    }

    // This is implicitly specify that data will be allocated and
    // using for the batching so allocate the data as much
    // as max buffer size since we're using a classic allocator model
    // like if we need a new buffer (is actually be allocated at the beginning)
    // it can be easily occupied by glNamedBufferSubData()
    Buffer &setBufferMaxCount(Gfx_u32 b = 32) {
        m_BufferMaxCount = b;
        m_BatchedData = true;
        return *this;
    }

private:
    //TODO: IBO will be implemented for the indices..
    void allocateGLBuffer(const VertexDescriptor<T> &vertexDesc, Gfx_enum bufferType);

    VertexDescriptor<T> m_VertDesc;
    BufferType m_BufferType;

    bool m_BufferInitialized{false};
    bool m_BatchedData{false};

    Gfx_u32 m_BufferMaxCount{};

    Gfx_u32 m_VBO{};
    Gfx_u32 m_VAO{};
    Gfx_u32 m_EBO{};
    Gfx_u32 m_ShaderProgram{};
};

template<typename T>
void Buffer<T>::initBuffer() {
    ALIEN_ASSERT(m_VertDesc.vertices.size() % m_VertDesc.vertexCountPerIndex != 1 &&
                 "Vertices count must be divisable by vertex count per index!");

    switch (GfxBase::SelectedGfxDeviceType) {
        case GFX_OGL: {
            allocateGLBuffer(m_VertDesc, m_BufferType);
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

template<typename T>
void Buffer<T>::initBuffer(BufferType bufferType) {
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

template<typename T>
void Buffer<T>::initBuffer(const VertexDescriptor<T> &vertexDescriptor, BufferType bufferType) {
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

template<typename T>
void Buffer<T>::allocateGLBuffer(const VertexDescriptor<T> &vertexDesc, Gfx_enum bufferType) {
    if (bufferType == BufferType::STATIC) {
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
            glVertexAttribPointer(posLoc, vertexDesc.vertexCountPerIndex, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) 0);
            glEnableVertexAttribArray(posLoc);

            glVertexAttribPointer(normalLoc, 3, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) (verticesSize * sizeof(double)));
            glEnableVertexAttribArray(normalLoc);

            glVertexAttribPointer(texCoordLoc, 2, GL_DOUBLE, GL_FALSE, 0,
                                  (Gfx_void *) ((normalsSize + verticesSize) * sizeof(double)));
            glEnableVertexAttribArray(texCoordLoc);
        } else if (std::is_same<T, float>::value) {
            glVertexAttribPointer(posLoc, vertexDesc.vertexCountPerIndex, GL_FLOAT, GL_FALSE, 0, (Gfx_void *) 0);
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
    } else if (bufferType == BufferType::DYNAMIC) {
        //TODO: For now, dynamic buffers are needed by
        // BatcheableSprites.
        auto payload = m_VertDesc.asInterleavedArray();

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        if (vertexDesc.hasIndices())
            glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        if (m_BatchedData) {
            glBufferData(GL_ARRAY_BUFFER, payload.size() * m_BufferMaxCount * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
        } else {
            glBufferData(GL_ARRAY_BUFFER, payload.size() * sizeof(T), &payload[0], GL_DYNAMIC_DRAW);
        }
        glCheckError();

        if (vertexDesc.hasIndices()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            if (m_BatchedData) {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexDesc.indices.size() * m_BufferMaxCount * sizeof(Gfx_u32),
                             nullptr, GL_DYNAMIC_DRAW);
            } else {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexDesc.indices.size() * sizeof(Gfx_u32),
                             &vertexDesc.indices[0], GL_DYNAMIC_DRAW);
            }
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
        glBindAttribLocation(this->m_ShaderProgram, 3, "textureId");

        Gfx_u32 posLoc = 0;
        Gfx_u32 normalLoc = 1;
        Gfx_u32 texCoordLoc = 2;
        Gfx_u32 texIdLoc = 3;

        static int stride = (6 + vertexDesc.vertexCountPerIndex) * sizeof(float);
        if (std::is_same<T, double>::value) {
            glEnableVertexAttribArray(posLoc);
            glVertexAttribPointer(posLoc, vertexDesc.vertexCountPerIndex, GL_DOUBLE, GL_FALSE, stride,
                                  (Gfx_void *) 0);

            glEnableVertexAttribArray(normalLoc);
            glVertexAttribPointer(normalLoc, 3, GL_DOUBLE, GL_FALSE, stride,
                                  (Gfx_void *) (vertexDesc.vertexCountPerIndex * sizeof(float)));

            glEnableVertexAttribArray(texCoordLoc);
            glVertexAttribPointer(texCoordLoc, 2, GL_DOUBLE, GL_FALSE, stride,
                                  (Gfx_void *) ((vertexDesc.vertexCountPerIndex + 3) * sizeof(float)));

            glEnableVertexAttribArray(texIdLoc);
            glVertexAttribPointer(texIdLoc, 1, GL_DOUBLE, GL_FALSE, stride,
                                  (Gfx_void *) ((vertexDesc.vertexCountPerIndex + 5) * sizeof(float)));
        } else if (std::is_same<T, float>::value) {

            glEnableVertexAttribArray(posLoc);
            glVertexAttribPointer(posLoc, vertexDesc.vertexCountPerIndex, GL_FLOAT, GL_FALSE, stride,
                                  (Gfx_void *) 0);

            glEnableVertexAttribArray(normalLoc);
            glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, stride,
                                  (Gfx_void *) (vertexDesc.vertexCountPerIndex * sizeof(float)));

            glEnableVertexAttribArray(texCoordLoc);
            glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, stride,
                                  (Gfx_void *) ((vertexDesc.vertexCountPerIndex + 3) * sizeof(float)));

            glEnableVertexAttribArray(texIdLoc);
            glVertexAttribPointer(texIdLoc, 1, GL_FLOAT, GL_FALSE, stride,
                                  (Gfx_void *) ((vertexDesc.vertexCountPerIndex + 5) * sizeof(float)));
        } else {
            static_assert("Only float and double type of vertex descriptor is allowed!");
        }

        m_BufferInitialized = true;
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }
}

#endif //ALIEN3D_BUFFER_H