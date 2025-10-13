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
    
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
    
    struct Vertexbuffer {
        Mesh* mesh;
        uint32_t vao, vbo, ebo;
    };
    
    void bindVertexbuffer(const Vertexbuffer &buffer);

    void unbindVertexbuffer();

    Vertexbuffer createVertexbuffer(Mesh* mesh);
}

#endif