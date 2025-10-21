#include "brl_vertexbuffer.hpp"
#include <cstring>
#include <iostream>

namespace brl {
    void bindVertexbuffer(const Vertexbuffer &buffer) {
        glBindVertexArray(buffer.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    }

    void bindInstancedVertexBuffer(const InstancedVertexBuffer &buffer) {
        glBindVertexArray(buffer.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    }

    void unbindVertexbuffer() {
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

    // void setInstancedVertexBufferData(const InstancedVertexBuffer &buffer, const InstanceData *instanceData, const uint32_t amount) {
    //     glBindBuffer(GL_ARRAY_BUFFER, buffer.instancebo);
    //     glBufferData(GL_ARRAY_BUFFER, amount * sizeof(InstanceData), &instanceData[0], GL_DYNAMIC_DRAW);
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    // }

    void setInstancedVertexBufferData(const InstancedVertexBuffer &vertexBuffer, const InstanceDataBuffer &dataBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.instancebo);
        glBufferData(GL_ARRAY_BUFFER, dataBuffer.used * sizeof(InstanceData), dataBuffer.data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void drawVertexBuffer(const Vertexbuffer &buffer) {
        bindVertexbuffer(buffer);
        glDrawElements(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, nullptr);
        unbindVertexbuffer();
    }

    void drawInstancedVertexBuffer(const InstancedVertexBuffer &buffer, const uint32_t amount) {
        bindInstancedVertexBuffer(buffer);
        glDrawElementsInstanced(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, 0, amount);
        unbindVertexbuffer();
    }

    Mesh createMesh(MeshData* meshData) {
        return Mesh{createVertexbuffer(meshData)};
    }

    InstanceDataBuffer createInstanceDataBuffer(const uint32_t size) {
        InstanceDataBuffer dataBuffer;
        dataBuffer.data = new InstanceData[size];
        dataBuffer.size = size;
        dataBuffer.used = 0;

        return dataBuffer;
    }

    void resizeInstanceDataBuffer(InstanceDataBuffer *buffer, const uint32_t size) {
        buffer->data = (InstanceData*)realloc(buffer->data, size*sizeof(InstanceData));
        buffer->size = size;

        if (buffer->used > size)
            buffer->used = size;
    }

    void addToInstanceDataBuffer(InstanceDataBuffer *buffer, const InstanceData &data) {
        if (buffer->used >= buffer->size)
            resizeInstanceDataBuffer(buffer, buffer->size*2);

        buffer->data[buffer->used] = data;
        buffer->used += 1;
    }

    void addArrayToInstanceDatabuffer(InstanceDataBuffer *buffer, const InstanceData *data, const uint32_t size) {
        if ((buffer->used + size) > buffer->size) {
            resizeInstanceDataBuffer(buffer, buffer->used + size);
        }
        memcpy(&buffer->data[buffer->used], data, size*sizeof(InstanceData));
        buffer->used += size;
    }

    void clearInstanceDataBuffer(InstanceDataBuffer *buffer) {
        buffer->used = 0;
    }

    void setInstanceDataBuffer(InstanceDataBuffer *buffer, InstanceData* data, uint32_t size) {
        buffer->data = data;
        buffer->size = size;
        buffer->used = size;
    }
}