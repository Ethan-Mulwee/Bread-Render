#include "brl_vertexbuffer.hpp"

namespace brl {
    void bindVertexbuffer(const Vertexbuffer &buffer) {
        glBindVertexArray(buffer.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    }

    void unbindVertexBuffer() {
        glBindVertexArray(0);
    }

    Vertexbuffer createVertexbuffer(const MeshData *mesh) {
        Vertexbuffer buffer;

        buffer.size = mesh->indices.size();

        glGenBuffers(1, &buffer.vbo);
        glGenBuffers(1, &buffer.ebo);
        glGenVertexArrays(1, &buffer.vao);
        
        glBindVertexArray(buffer.vao);

        glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint32_t), mesh->indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return buffer;
    }

    void drawVertexBuffer(const Vertexbuffer &buffer) {
        // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        // glEnable(GL_CULL_FACE);
        bindVertexbuffer(buffer);
        glDrawElements(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, nullptr);
        unbindVertexBuffer();
    }
}