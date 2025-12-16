#ifndef BRL_VIEWPORT
#define BRL_VIEWPORT

#include "brl_render.hpp"
#include "brl_framebuffer.hpp"
#include "brl_camera.hpp"

namespace brl {
    struct ViewportContext {

        Framebuffer framebuffer;
        Framebuffer output_framebuffer;
        uint32_t MSAA = 0;

        RenderContext* render_context;
        ImVec2 screen_position;
        ImVec2 position;
        ImVec2 size;
        bool hovered;
        bool focused;
        const char* name;
        brl::Color background_color = brl::GREY;
    };

    ViewportContext create_viewport_context(RenderContext* renderContext, const char* name, const uint32_t MSAA = 4);

    void begin_viewport(ViewportContext &viewport, Camera &camera);
    void end_viewport(ViewportContext &viewport, Camera &camera);

    void set_viewport_background(ViewportContext* viewportContext, const Color &color);
}

#endif