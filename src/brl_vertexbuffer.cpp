#include "brl_vertexbuffer.hpp"

namespace brl {
    void bindVertexbuffer(const Vertexbuffer &buffer) {
        glBindVertexArray(buffer.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    }

    void bindInstancedVertexBuffer(const InstancedVertexBuffer &buffer) {
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

    InstancedVertexBuffer createInstancedVertexBuffer(const MeshData *mesh) {
        InstancedVertexBuffer buffer;

        buffer.size = mesh->indices.size();

        glGenBuffers(1, &buffer.vbo);
        glGenBuffers(1, &buffer.ebo);
        glGenBuffers(1, &buffer.instancebo);
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

        glBindBuffer(GL_ARRAY_BUFFER, buffer.instancebo);

        // Adapted from https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/4.advanced_opengl/10.3.asteroids_instanced/asteroids_instanced.cpp
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(smath::matrix4x4), (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(smath::matrix4x4), (void*)(sizeof(smath::vector4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(smath::matrix4x4), (void*)(2 * sizeof(smath::vector4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(smath::matrix4x4), (void*)(3 * sizeof(smath::vector4)));

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return buffer;
    }

    void setInstancedVertexBufferData(const InstancedVertexBuffer &buffer, const smath::matrix4x4 *transforms, const uint32_t amount) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer.instancebo);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(smath::matrix4x4), &transforms[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void drawVertexBuffer(const Vertexbuffer &buffer) {
        // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        // glEnable(GL_CULL_FACE);
        bindVertexbuffer(buffer);
        glDrawElements(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, nullptr);
        unbindVertexBuffer();
    }

    Mesh createMesh(MeshData* meshData) {
        return Mesh{createVertexbuffer(meshData)};
    }
}