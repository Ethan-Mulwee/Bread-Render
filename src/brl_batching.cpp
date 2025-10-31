#include "brl_batching.hpp"

#include <iostream>

namespace brl {
    void Batch::clear() {
        used = 0;
    }

    void Batch::resize(uint new_capacity) {
        elements = (BatchElement*)realloc(elements, new_capacity * sizeof(BatchElement));
        capacity = new_capacity;
        if (used > capacity)
            used = capacity;
    }

    void Batch::add(const BatchElement &element) {
        if (used >= capacity)
            resize(capacity * 2);

        elements[used] = element;
        used++;
    }

    Batch createBatch(uint capacity, const Vertexbuffer &vertexBuffer) {
        Batch batch;
        batch.elements = new BatchElement[capacity];
        batch.capacity = capacity;
        batch.used = 0;

        batch.vertexBuffer = vertexBuffer;

        return batch;
    }

    void Batcher::clear() {
        dynamic_used = 0;

        for (int i = 0; i < dynamic_capactiy; i++)
            dynamic_batches[i].clear();
    }

    void Batcher::resize(uint new_capacity) {
        dynamic_batches = (Batch*)realloc(dynamic_batches, new_capacity * sizeof(Batch));
        dynamic_capactiy = new_capacity;
        if (dynamic_used > dynamic_capactiy)
            dynamic_used = dynamic_capactiy;
    }

    void Batcher::add(const Vertexbuffer &vertexbuffer, const smath::matrix4x4 &transform, const smath::vector4 &color) {

        uint id = vertexbuffer.vao;

        if (id > dynamic_used)
            dynamic_used = vertexbuffer.vao;

        if (dynamic_used >= dynamic_capactiy)
            resize(dynamic_capactiy * 2);

        if (dynamic_batches[id].elements) {
            dynamic_batches[id].add(BatchElement{transform, color});
        } else {
            dynamic_batches[id] = createBatch(50, vertexbuffer);
            dynamic_batches[id].add(BatchElement{transform, color});
        }
    }

    Batcher createBatcher(uint capacity) {
        Batcher batcher;

        batcher.dynamic_batches = new Batch[capacity];
        batcher.dynamic_capactiy = capacity;
        batcher.dynamic_used = 0;

        batcher.static_batches = new Batch[capacity];
        batcher.static_capactiy = capacity;
        batcher.static_used = 0;

        return batcher;
    }
}