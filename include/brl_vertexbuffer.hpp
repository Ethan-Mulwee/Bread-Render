#ifndef BRL_VERTEXBUFFER
#define BRL_VERTEXBUFFER

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <cstdint>

#include "smath.hpp"

namespace brl {
    struct Vertex {
        smath::vector3 position;
        smath::vector3 normal; 
    };
    
    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
    
    struct Vertexbuffer {
        uint32_t size;
        uint32_t vao, vbo, ebo;
    };

    struct InstancedVertexBuffer {
        uint32_t size;
        uint32_t vao, vbo, ebo, instancebo;
    };

    struct InstanceData {
        smath::matrix4x4 transform;
        smath::vector4 color;
    };

    struct InstanceDataBuffer {
        InstanceData* data;
        uint32_t size;
        uint32_t used;
    };

    InstanceDataBuffer createInstanceDataBuffer(const uint32_t size);
    void resizeInstanceDataBuffer(InstanceDataBuffer* buffer, const uint32_t size);
    void addToInstanceDataBuffer(InstanceDataBuffer* buffer, const InstanceData &data);
    void clearInstanceDataBuffer(InstanceDataBuffer* buffer);
    // set InstanceDataBuffer as some provided array to avoid overhead of copying
    void setInstanceDataBuffer(InstanceDataBuffer *buffer, InstanceData* data, uint32_t size);

    struct Mesh {
        Vertexbuffer buffer;
    };

    Mesh createMesh(MeshData* meshData);

    struct Model {
        Mesh mesh;
        smath::matrix4x4 transform;
        smath::vector4 color;
    };
    
    void bindVertexbuffer(const Vertexbuffer &buffer);

    void unbindVertexbuffer();

    Vertexbuffer createVertexbuffer(const MeshData* mesh);

    InstancedVertexBuffer createInstancedVertexBuffer(const MeshData* mesh);
    void setInstancedVertexBufferData(const InstancedVertexBuffer &buffer, const InstanceData* instanceData, const uint32_t amount);
    void bindInstancedVertexBuffer(const InstancedVertexBuffer &buffer);

    void drawVertexBuffer(const Vertexbuffer &buffer);
}

#endif