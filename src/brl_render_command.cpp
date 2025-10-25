#include "brl_render_command.hpp"

namespace brl {
    RenderCommandBuffer createRenderCommandBuffer(uint32_t modelCapactiy, uint32_t sceneCapacity) {
        RenderCommandBuffer commandBuffer {
            .modelBuffer = new Model[modelCapactiy],
            .modelBufferCapacity = modelCapactiy,
            .modelBufferUsed = 0,

            .sceneDataBuffer = new SceneData*[sceneCapacity],
            .sceneBufferCapacity = sceneCapacity,
            .sceneBufferUsed = 0
        };

        return commandBuffer;
    }

    // void addRenderCommand(RenderCommandBuffer *buffer, const RenderCommand &renderCommand) {
        // if (buffer->used >= buffer->capacity)
            // return;

        // buffer->buffer[buffer->used] = renderCommand;
        // buffer->used += 1;
    // }

    void renderBufferAddModel(RenderCommandBuffer* buffer, const Model &model) {
        if (buffer->modelBufferUsed >= buffer->modelBufferCapacity)
            return;

        buffer->modelBuffer[buffer->modelBufferUsed] = model;
        buffer->modelBufferUsed += 1;
    }

    void renderBufferAddScene(RenderCommandBuffer* buffer, SceneData* scene) {
        if (buffer->sceneBufferUsed >= buffer->sceneBufferCapacity)
            return;

        buffer->sceneDataBuffer[buffer->sceneBufferUsed] = scene;
        buffer->sceneBufferUsed += 1;
    }

    void resetRenderCommandBuffer(RenderCommandBuffer* buffer) {
        buffer->modelBufferUsed = 0;
        buffer->sceneBufferUsed = 0;
    }
}