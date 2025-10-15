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
    };

    ViewportContext createViewportContext(const RenderContext* renderContext, int width, int height);

    void beginViewport(const ViewportContext &viewport, const Camera &camera);
    void endViewport(const ViewportContext &viewport, const Camera &camera);
}

#endif