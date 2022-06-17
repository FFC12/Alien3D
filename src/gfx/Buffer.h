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
    std::initializer_list<T> vertices;

    std::initializer_list<T> colors;
    // 2
    std::initializer_list<T> normals;

    // 2
    std::initializer_list<T> textureCoord;
    std::initializer_list<Gfx_u32> indices;

    size_t vertexCountPerIndex;

    VertexDescriptor(const std::initializer_list<T> &vertices,
                     const std::initializer_list<T> &colors,
                     const std::initializer_list<T> &normals,
                     const std::initializer_list<T> &textureCoord,
                     size_t vertexCount = 3)
            : vertices(vertices), colors(colors), normals(normals), textureCoord(textureCoord),
              vertexCountPerIndex(vertexCount) {}

    VertexDescriptor(const std::initializer_list<T> &vertices,
                     const std::initializer_list<T> &colors,
                     const std::initializer_list<T> &normals,
                     const std::initializer_list<T> &textureCoord,
                     bool hasIndices,
                     const std::initializer_list<Gfx_u32> &indices,
                     size_t vertexCount = 3)
            : vertices(vertices),
              colors(colors),
              normals(normals),
              textureCoord(textureCoord),
              m_hasIndices(hasIndices),
              indices(indices),
              vertexCountPerIndex(vertexCount) {}

    VertexDescriptor(const VertexDescriptor<T> &vertexDescriptor) {
        vertices = vertexDescriptor.vertices;
        colors = vertexDescriptor.colors;
        normals = vertexDescriptor.normals;
        textureCoord = vertexDescriptor.textureCoord;
        m_hasIndices = vertexDescriptor.hasIndices();
        vertexCountPerIndex = vertexDescriptor.vertexCountPerIndex;
    }

    VertexDescriptor<T> &operator=(const VertexDescriptor<T> &vertexDescriptor) {
        vertices = vertexDescriptor.vertices;
        colors = vertexDescriptor.colors;
        normals = vertexDescriptor.normals;
        textureCoord = vertexDescriptor.textureCoord;
        m_hasIndices = vertexDescriptor.hasIndices();
        vertexCountPerIndex = vertexDescriptor.vertexCountPerIndex;

        return *this;
    }

    [[nodiscard]]
    bool hasIndices() const {
        return m_hasIndices;
    }

private:
    bool m_hasIndices{};
};

enum BufferType {
    STATIC,
    DYNAMIC,
    STREAM
};

template<typename T>
class Buffer {
public:
    Buffer() {}

    Buffer(const VertexDescriptor<T> &vertexDescriptor, BufferType bufferType)
    //       : m_VertDesc(vertexDescriptor)
    {
        ALIEN_ASSERT(vertexDescriptor.vertices.size() % vertexDescriptor.vertexCountPerIndex != 1 &&
                     "Vertices count must be divisable by vertex count per index!");

        this->m_BufferType = bufferType;
    }

/*
    Buffer& process() {
        ALIEN_ASSERT(this->m_ShaderProgram == 0 && "Shader program must be set!");

        Gfx_enum bufferDrawType;
        switch (this->m_BufferType) {
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
                allocateGLBuffer(this->m_VertDesc,bufferDrawType);
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
        return *this;
    }
*/

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

    void useProgram() {
        glUseProgram(this->m_ShaderProgram);
        glCheckError();
    }

    Buffer &setProgram(Gfx_u32 program) {
        this->m_ShaderProgram = program;
        return *this;
    }

    Gfx_u32 getVAO() {
        return this->m_VAO;
    }

private:
    //TODO: IBO will be implemented for the indices..
    void allocateGLBuffer(const VertexDescriptor<T> &vertexDesc, Gfx_enum bufferType) {
        //3
        std::vector<T> payload(vertexDesc.vertices.begin(), vertexDesc.vertices.end());

        //3
        payload.insert(payload.end(), vertexDesc.colors.begin(), vertexDesc.colors.end());

        //2
        payload.insert(payload.end(), vertexDesc.normals.begin(), vertexDesc.normals.end());

        //2
        payload.insert(payload.end(), vertexDesc.textureCoord.begin(), vertexDesc.textureCoord.end());

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
                         vertexDesc.indices.begin(), bufferType);
            glCheckError();
        }

        /*
        auto posLoc = glGetAttribLocation(this->m_ShaderProgram, "position");
        auto colorLoc = glGetAttribLocation(this->m_ShaderProgram, "color");
        auto normalLoc = glGetAttribLocation(this->m_ShaderProgram, "normal");
        auto texCoordLoc = glGetAttribLocation(this->m_ShaderProgram, "texCoord");
         */
        glBindAttribLocation(this->m_ShaderProgram, 0, "position");
        glBindAttribLocation(this->m_ShaderProgram, 1, "color");
        glBindAttribLocation(this->m_ShaderProgram, 2, "normal");
        glBindAttribLocation(this->m_ShaderProgram, 3, "texCoord");

        Gfx_u32 posLoc = 0;
        Gfx_u32 colorLoc = 1;
        Gfx_u32 normalLoc = 2;
        Gfx_u32 texCoordLoc = 3;

        size_t verticesSize = vertexDesc.vertices.size();
        size_t colorsSize = vertexDesc.colors.size();
        size_t normalsSize = vertexDesc.normals.size();
//        size_t texCoordSize = vertexDesc.textureCoord.size();

        if (std::is_same<T, double>::value) {
            glVertexAttribPointer(posLoc, 3, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) 0);
            glEnableVertexAttribArray(posLoc);

            glVertexAttribPointer(colorLoc, 3, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) (verticesSize * sizeof(double)));
            glEnableVertexAttribArray(colorLoc);

            glVertexAttribPointer(normalLoc, 2, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) (colorsSize * sizeof(double)));
            glEnableVertexAttribArray(normalLoc);

            glVertexAttribPointer(texCoordLoc, 2, GL_DOUBLE, GL_FALSE, 0, (Gfx_void *) (normalsSize * sizeof(double)));
            glEnableVertexAttribArray(texCoordLoc);
        } else if (std::is_same<T, float>::value) {
            glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (Gfx_void *) 0);
            glEnableVertexAttribArray(posLoc);

            glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, (Gfx_void *) (verticesSize * sizeof(float)));
            glEnableVertexAttribArray(colorLoc);

            glVertexAttribPointer(normalLoc, 2, GL_FLOAT, GL_FALSE, 0,
                                  (Gfx_void *) ((colorsSize + verticesSize) * sizeof(float)));
            glEnableVertexAttribArray(normalLoc);

            glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0,
                                  (Gfx_void *) ((normalsSize + colorsSize + verticesSize) * sizeof(float)));
            glEnableVertexAttribArray(texCoordLoc);
        } else {
            static_assert("Only float and double type of vertex descriptor is allowed!");
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

//    VertexDescriptor<T>& m_VertDesc;
    BufferType m_BufferType;
    Gfx_u32 m_VBO{};
    Gfx_u32 m_VAO{};
    Gfx_u32 m_EBO{};

    Gfx_u32 m_ShaderProgram{};
};


#endif //ALIEN3D_BUFFER_H
