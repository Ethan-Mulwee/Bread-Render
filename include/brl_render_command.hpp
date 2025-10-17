#ifndef BRL_RENDER_COMMAND
#define BRL_RENDER_COMMAND

#include <cstdint>
#include "smath.hpp"

namespace brl {
    struct RenderCommand {
        smath::matrix4x4 transform;
    };
    
    struct RenderCommandBuffer {
        uint32_t capacity = 0;
        uint32_t used = 0;
        RenderCommand* buffer;
    };

    void addRenderCommand(RenderCommandBuffer* buffer, const RenderCommand &command);
    void resetRenderCommandBuffer(RenderCommandBuffer* buffer);

    RenderCommandBuffer createRenderCommandBuffer(uint32_t capactiy);
}

#endif