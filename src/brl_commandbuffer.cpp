#include "brl_commandbuffer.hpp"

namespace brl {
    void RenderGroup::clear() {
        used = 0;
    }

    void RenderGroup::resize(uint new_capacity) {
        elements = (ModelData*)realloc(elements, new_capacity * sizeof(ModelData));
        capacity = new_capacity;
        if (used > capacity)
            used = capacity;

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(ModelData), nullptr, GL_DYNAMIC_DRAW);
    }

    void RenderGroup::add(const ModelData &element) {
        if (used >= capacity)
            resize(capacity * 2);

        elements[used] = element;
        used++;
    }

    void RenderGroup::set(const Vertexbuffer &new_vertexbuffer, ModelData* new_elements, const uint32_t count) {
        if (count > capacity) {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.instanceVBO);
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(ModelData), nullptr, GL_DYNAMIC_DRAW);
        }

        capacity = count;
        used = count;
        elements = new_elements;
        vertexBuffer = new_vertexbuffer;
    }

    RenderGroup createBatch(uint capacity, const Vertexbuffer &vertexBuffer) {
        RenderGroup batch;
        batch.elements = new ModelData[capacity];
        batch.capacity = capacity;
        batch.used = 0;

        batch.vertexBuffer = vertexBuffer;

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(ModelData), nullptr, GL_DYNAMIC_DRAW);

        return batch;
    }

    void RenderGrouper::clear() {
        dynamic_used = 0;
        static_used = 0;

        for (int i = 0; i < dynamic_capactiy; i++)
            dynamic_batches[i].clear();

        
    }

    void RenderGrouper::resize(uint new_capacity) {
        dynamic_batches = (RenderGroup*)realloc(dynamic_batches, new_capacity * sizeof(RenderGroup));
        dynamic_capactiy = new_capacity;
        if (dynamic_used > dynamic_capactiy)
            dynamic_used = dynamic_capactiy;
    }

    void RenderGrouper::addDynamic(const Vertexbuffer &vertexbuffer, const smath::matrix4x4 &transform, const smath::vector4 &color) {

        uint id = vertexbuffer.vao;

        if (id > dynamic_used)
            dynamic_used = vertexbuffer.vao+1;

        if (dynamic_used >= dynamic_capactiy)
            resize(dynamic_capactiy * 2);

        if (dynamic_batches[id].elements) {
            dynamic_batches[id].add(ModelData{transform, color});
        } else {
            dynamic_batches[id] = createBatch(50, vertexbuffer);
            dynamic_batches[id].add(ModelData{transform, color});
        }
    }

    void RenderGrouper::addStatic(const Vertexbuffer &vertexbuffer, ModelData* elements, const uint32_t count) {

        uint id = vertexbuffer.vao;
        while(static_batches[id].elements && id <= static_used) {
            id++;
        }

        if (id > static_used)
            static_used = id+1;

        if (static_used >= static_capactiy)
            resize(static_capactiy * 2);

        static_batches[id].set(vertexbuffer, elements, count);
    }

    RenderGrouper createBatcher(uint capacity) {
        RenderGrouper batcher;

        batcher.dynamic_batches = new RenderGroup[capacity];
        batcher.dynamic_capactiy = capacity;
        batcher.dynamic_used = 0;

        batcher.static_batches = new RenderGroup[capacity];
        batcher.static_capactiy = capacity;
        batcher.static_used = 0;

        return batcher;
    }
}