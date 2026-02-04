#include "brl_vertexbuffer.hpp"
#include <cstring>
#include <iostream>

namespace brl {
    void bind_vertexbuffer(const Vertexbuffer &buffer) {
        glBindVertexArray(buffer.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    }

    // void bindInstancedVertexBuffer(const InstancedVertexBuffer &buffer) {
    //     glBindVertexArray(buffer.vao);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    // }

    void unbind_vertexbuffer() {
        glBindVertexArray(0);
    }

    Vertexbuffer create_vertexbuffer(const MeshData *mesh) {
        Vertexbuffer buffer;

        buffer.size = mesh->indices.size();

        glGenBuffers(1, &buffer.vbo);
        glGenBuffers(1, &buffer.ebo);
        glGenBuffers(1, &buffer.instanceVBO);
        glGenVertexArrays(1, &buffer.vao);

        glBindVertexArray(buffer.vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint32_t), mesh->indices.data(), GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, buffer.instanceVBO);
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, transform));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + sizeof(smath::vector4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 2 * sizeof(smath::vector4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 3 * sizeof(smath::vector4)));

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
        glVertexAttribDivisor(6, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return buffer;
    }

    DynamicVertexbuffer create_dynamic_vertexbuffer(const MeshData *mesh) {
        DynamicVertexbuffer buffer;

        buffer.size = mesh->indices.size();

        glGenBuffers(1, &buffer.vbo);
        glGenBuffers(1, &buffer.ebo);
        glGenBuffers(1, &buffer.instanceVBO);
        glGenVertexArrays(1, &buffer.vao);

        glBindVertexArray(buffer.vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint32_t), mesh->indices.data(), GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, buffer.instanceVBO);
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, transform));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + sizeof(smath::vector4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 2 * sizeof(smath::vector4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 3 * sizeof(smath::vector4)));

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
        glVertexAttribDivisor(6, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return buffer;
    }

    void draw_vertex_buffer(const Vertexbuffer &buffer) {
        bind_vertexbuffer(buffer);
        glDrawElements(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, nullptr);
        unbind_vertexbuffer();
    }

    Vertexbuffer createVertexbuffer(const MeshData *mesh) {
        Vertexbuffer buffer;

        buffer.size = mesh->indices.size();

        glGenBuffers(1, &buffer.vbo);
        glGenBuffers(1, &buffer.ebo);
        glGenBuffers(1, &buffer.instanceVBO);
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

        glBindBuffer(GL_ARRAY_BUFFER, buffer.instanceVBO);
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, transform));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + sizeof(smath::vector4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 2 * sizeof(smath::vector4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 3 * sizeof(smath::vector4)));

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
        glVertexAttribDivisor(6, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return buffer;
    }

    
    // void drawVertexbufferInstanced(const Vertexbuffer &buffer, const ModelData *data, const uint32_t count) {
    //     glBindBuffer(GL_ARRAY_BUFFER, buffer.instanceVBO);
    //     glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(ModelData), data);

    //     bind_vertexbuffer(buffer);
    //     glDrawElementsInstanced(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, 0, count);
    //     unbind_vertexbuffer();
    // }

    void draw_vertexbuffer_instanced(const Vertexbuffer &buffer, const InstanceData *data, const uint32_t count) {
        uint32_t instanceVBO;
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(InstanceData), data, GL_DYNAMIC_DRAW);

        glBindVertexArray(buffer.vao);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, transform));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + sizeof(smath::vector4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 2 * sizeof(smath::vector4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(offsetof(InstanceData, transform) + 3 * sizeof(smath::vector4)));

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
        glVertexAttribDivisor(6, 1);

        bind_vertexbuffer(buffer);
        glDrawElementsInstanced(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, 0, count);
        unbind_vertexbuffer();
    }

    // Call sync on GLSyncBuffer before using this to prepare instance data
    void draw_vertexbuffer_instanced2(const Vertexbuffer &buffer, const InstanceData *data, const uint32_t count) {
        bind_vertexbuffer(buffer);
        glDrawElementsInstanced(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, 0, count);
        unbind_vertexbuffer();
    }

    Mesh create_mesh(MeshData* meshData) {
        return Mesh{create_vertexbuffer(meshData)};
    }
}