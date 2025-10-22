#ifndef BRL_VIEWPORT
#define BRL_VIEWPORT

#include "brl_render.hpp"
#include "brl_framebuffer.hpp"
#include "brl_camera.hpp"

namespace brl {
    struct ViewportContext {
        Framebuffer framebuffer;
        RenderContext* renderContext;
        ImVec2 screenPosition;
        ImVec2 position;
        ImVec2 size;
        bool hovered;
        bool focused;
        const char* name;
    };

    ViewportContext createViewportContext(RenderContext* renderContext, const char* name);

    void beginViewport(ViewportContext &viewport, Camera &camera);
    void endViewport(ViewportContext &viewport, Camera &camera);
}

#endif