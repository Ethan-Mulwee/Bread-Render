#ifndef BRL_RENDER
#define BRL_RENDER

#include "brl_window.hpp"
#include "brl_framebuffer.hpp"
#include "brl_vertexbuffer.hpp"
#include "brl_shader.hpp"

namespace brl {
    struct PrimtiveMeshes {

    };

    struct RenderContext {
        const Window* window;
        Framebuffer frameBuffer;
        Shader objectShader;
        Shader gridShader;
    };

    void beginRender(Window* window);
    void clearRender(smath::vector4 color = {0.2f, 0.2f, 0.2f, 0.2f});
    void endRender();

    void bindRenderContext(RenderContext context);
}

#endif