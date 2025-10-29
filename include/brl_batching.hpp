#ifndef BRL_DRAW_BUFFER
#define BRL_DRAW_BUFFER

#include <cstdint>
#include "brl_vertexbuffer.hpp"
#include "smath.hpp"

namespace brl {

    struct BatchElement {   
        float transform[16];
        float color[4];
    };

    struct Batch {
        BatchElement* elements = nullptr;
        Vertexbuffer vertexBuffer;
        uint capacity = 0;
        uint used = 0;

        void clear();

        void resize(uint new_capacity);

        void add(const BatchElement &element);
    };

    Batch createBatch(uint capacity);

    struct Batcher {
        Batch* batches;
        uint capactiy = 0;
        uint used = 0;

        void clear();

        void resize (uint new_capacity);

        void add(const BatchElement &batchElement);

        void addBatch(const Batch &batch);
    };

    Batcher createBatcher(uint capacity);

}

#endif