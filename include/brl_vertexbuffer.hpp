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

    struct InstanceData {
        smath::matrix4x4 transform;
        smath::vector4 color;
    };
    
    void bindVertexbuffer(const Vertexbuffer &buffer);

    void unbindVertexbuffer();

    Vertexbuffer createVertexbuffer(const MeshData* mesh);

    void drawVertexBuffer(const Vertexbuffer &buffer);
    
    void drawVertexbufferInstanced(const Vertexbuffer &buffer, const InstanceData* data, const uint32_t count);
}

#endif