#ifndef BRL_DRAW_BUFFER
#define BRL_DRAW_BUFFER

#include <cstdint>
#include "brl_vertexbuffer.hpp"
#include "smath.hpp"

namespace brl {

    template<typename T> 
    struct Pool {
        T* elements = nullptr;
        uint capacity = 0;
        uint used = 0;

        void clear() {
            used = 0;
        }

        void resize(uint new_capacity) {
            elements = (ModelData*)realloc(elements, new_capacity * sizeof(ModelData));
            capacity = new_capacity;
            if (used > capacity)
                used = capacity;
    
            // glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.instanceVBO);
            // glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(ModelData), nullptr, GL_DYNAMIC_DRAW);
        }

        void add(const T &element) {
            if (used >= capacity)
                resize(capacity * 2);
    
            elements[used] = element;
            used++;
        }
    };

    struct CommandPool {

    };

    struct RenderGroup {
        Pool<ModelData> dataPool;
        Pool<ModelData*> bufferPool;
        ModelData* elements = nullptr;
        Vertexbuffer vertexBuffer;
        uint capacity = 0;
        uint used = 0;

        void clear();

        void resize(uint new_capacity);

        void add(const ModelData &element);

        void set(const Vertexbuffer &new_vertexbuffer, ModelData* new_elements, const uint32_t count);
    };

    RenderGroup createBatch(uint capacity, const Vertexbuffer &vertexBuffer);

    struct RenderGrouper {
        RenderGroup* dynamic_batches;
        uint dynamic_capactiy = 0;
        uint dynamic_used = 0;
        RenderGroup* static_batches;
        uint static_capactiy = 0;
        uint static_used = 0;

        void clear();

        void resize (uint new_capacity);

        void addDynamic(const Vertexbuffer &vertexbuffer, const smath::matrix4x4 &transform, const smath::vector4 &color);

        void addStatic(const Vertexbuffer &vertexbuffer, ModelData* elements, const uint32_t count);

        void addBatch(const RenderGroup &batch);
    };

    RenderGrouper createBatcher(uint capacity);

}

#endif