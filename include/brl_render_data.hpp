#ifndef BRL_RENDER_DATA
#define BRL_RENDER_DATA

// temp for dynamic arrays make your own solution later
#include "brl_vertexbuffer.hpp"
#include "brl_color.hpp"
#include <vector>

namespace brl {
    using namespace smath;
    
    enum RenderConfiguration {
        SOLID, WIREFRAME, TRANSPARENT
    };

    struct RenderData {
        RenderConfiguration config;
        Vertexbuffer vertexbuffer;
        matrix4x4 transform;
        Color color;
    };

    /* An automatically growing linear buffer that tracks how many of it's elements are valid/used
    I.e. the data can be 'cleared' each frame without any memory operations it is just makred as free.

    Unlike std::vector calling clear() does not call the destructor on the elements inside
    therefore saving any allocated memory buffers inside for the next frame just invalidating their data */
    template<typename T>
    struct ScratchVector {
        T* elements;
        u_int32_t used;
        u_int32_t size;

        void add(T element);
        
        void reize();

        void shrink_to_fit();

    };

    struct ObjectRenderDataArray {
        Vertexbuffer vertexbuffer;
        std::vector<matrix4x4> transforms;
        std::vector<Color> colors;
    };

    struct ConfigurationRenderDataNode {
        RenderConfiguration config;
        std::vector<ObjectRenderDataArray> object_arrays;
    };

    struct DynamicRenderDataTree {

        std::vector<ConfigurationRenderDataNode> configurations;

        // Addes RenderData to the tree in it's corresponding leaf node i.e. ObjectRenderDataArray
        void add(const RenderData &data);

        // Called after each frame is rendered to invalidate all the data in the buffers of the tree without deallocating any memory
        // NOTE: check to see if clear() on a std::vector of a struct that contains more std::vector will deallocate any of the vectors which you don't want
        // it needs to be effiectlvey a recursive clear
        void clear();
    };

    struct BatchRenderDataTree {

    };

    struct SceneRenderDataTree {
        
    };
}

#endif