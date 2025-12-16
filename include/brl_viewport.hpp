#ifndef BRL_VIEWPORT
#define BRL_VIEWPORT

#include "brl_render.hpp"
#include "brl_framebuffer.hpp"
#include "brl_camera.hpp"

namespace brl {
    struct ViewportContext {

        Framebuffer framebuffer;
        Framebuffer outputFramebuffer;
        uint32_t MSAA = 0;

        RenderContext* renderContext;
        ImVec2 screenPosition;
        ImVec2 position;
        ImVec2 size;
        bool hovered;
        bool focused;
        const char* name;
        brl::Color background_color = brl::GREY;
    };

    ViewportContext createViewportContext(RenderContext* renderContext, const char* name, const uint32_t MSAA = 4);

    void beginViewport(ViewportContext &viewport, Camera &camera);
    void endViewport(ViewportContext &viewport, Camera &camera);

    void setViewportBackground(ViewportContext* viewportContext, const Color &color);
}

#endif