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

    struct Mesh {
        Vertexbuffer buffer;
    };

    Mesh create_mesh(MeshData* mesh_data);

    struct Model {
        Mesh mesh;
        smath::matrix4x4 transform;
        smath::vector4 color;
    };
    
    void bind_vertexbuffer(const Vertexbuffer &buffer);

    void unbind_vertexbuffer();

    Vertexbuffer create_vertexbuffer(const MeshData* mesh);

    void draw_vertex_buffer(const Vertexbuffer &buffer);
    
    void draw_vertexbuffer_instanced(const Vertexbuffer &buffer, const InstanceData* data, const uint32_t count);
}

#endif