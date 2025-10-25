#ifndef BRL_RENDER_COMMAND
#define BRL_RENDER_COMMAND

#include <cstdint>
#include "smath.hpp"
#include "brl_scene.hpp"

namespace brl {
    
    struct RenderCommandBuffer {
        Model* modelBuffer;
        uint32_t modelBufferCapacity = 0;
        uint32_t modelBufferUsed = 0;

        SceneData** sceneDataBuffer;
        uint32_t sceneBufferCapacity = 0;
        uint32_t sceneBufferUsed = 0;
    };

    void renderBufferAddModel(RenderCommandBuffer* buffer, const Model &model);
    void renderBufferAddScene(RenderCommandBuffer* buffer, SceneData* scene);
    void resetRenderCommandBuffer(RenderCommandBuffer* buffer);

    RenderCommandBuffer createRenderCommandBuffer(uint32_t modelCapactiy, uint32_t sceneCapacity = 1);
}

#endif