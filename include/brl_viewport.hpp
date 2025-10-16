#ifndef BRL_VIEWPORT
#define BRL_VIEWPORT

#include "brl_render.hpp"
#include "brl_framebuffer.hpp"
#include "brl_camera.hpp"

namespace brl {
    struct ViewportContext {
        Framebuffer framebuffer;
        const RenderContext* renderContext;
        ImVec2 position;
        ImVec2 size;
        const char* name;
    };

    ViewportContext createViewportContext(const RenderContext* renderContext, int width, int height, const char* name);

    void beginViewport(ViewportContext &viewport, Camera &camera);
    void endViewport(ViewportContext &viewport, Camera &camera);
}

#endif