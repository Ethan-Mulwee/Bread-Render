#ifndef BRL_RENDER_COMMAND
#define BRL_RENDER_COMMAND

#include <cstdint>
#include "smath.hpp"
#include "brl_scene.hpp"

namespace brl {
    struct RenderCommand {
        Model model;
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