#ifndef BRL_DRAW_BUFFER
#define BRL_DRAW_BUFFER

#include <cstdint>
#include "brl_vertexbuffer.hpp"
#include "smath.hpp"

namespace brl {

    struct Batch {
        ModelData* elements = nullptr;
        Vertexbuffer vertexBuffer;
        uint capacity = 0;
        uint used = 0;

        void clear();

        void resize(uint new_capacity);

        void add(const ModelData &element);

        void set(const Vertexbuffer &new_vertexbuffer, ModelData* new_elements, const uint32_t count);
    };

    Batch createBatch(uint capacity, const Vertexbuffer &vertexBuffer);

    struct Batcher {
        Batch* dynamic_batches;
        uint dynamic_capactiy = 0;
        uint dynamic_used = 0;
        Batch* static_batches;
        uint static_capactiy = 0;
        uint static_used = 0;

        void clear();

        void resize (uint new_capacity);

        void addDynamic(const Vertexbuffer &vertexbuffer, const smath::matrix4x4 &transform, const smath::vector4 &color);

        void addStatic(const Vertexbuffer &vertexbuffer, ModelData* elements, const uint32_t count);

        void addBatch(const Batch &batch);
    };

    Batcher createBatcher(uint capacity);

}

#endif