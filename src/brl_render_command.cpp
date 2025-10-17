#include "brl_render_command.hpp"

namespace brl {
    RenderCommandBuffer createRenderCommandBuffer(uint32_t capactiy) {
        RenderCommandBuffer commandBuffer {
            .capacity = capactiy,
            .used = 0,
            .buffer = new RenderCommand[capactiy]
        };

        return commandBuffer;
    }

    void addRenderCommand(RenderCommandBuffer *buffer, const RenderCommand &renderCommand) {
        if (buffer->used >= buffer->capacity)
            return;

        buffer->buffer[buffer->used] = renderCommand;
        buffer->used += 1;
    }

    void resetRenderCommandBuffer(RenderCommandBuffer* buffer) {
        buffer->used = 0;
    }
}