#ifndef BRL_VIEWPORT
#define BRL_VIEWPORT

#include "brl_render.hpp"
#include "brl_framebuffer.hpp"
#include "brl_camera.hpp"

namespace brl {

    struct ViewportSettings {
        bool shadowMap = true;
        uint32_t shadowMapResoltuion = 1000;
        bool shadowPCF = true;
        bool shadowPossion = true;
        uint32_t PossionSamples = 8; 
        uint32_t MSAA = 4;
    };

    struct ViewportContext {
        const char* name;
        ViewportSettings settings;

        Framebuffer renderFramebuffer;
        Framebuffer shadowFramebuffer;
        Framebuffer outputFramebuffer;

        RenderContext* renderContext;

        // this data is dynamically updated by viewport functions
        ImVec2 screenPosition;
        ImVec2 position;
        ImVec2 size;
        bool hovered;
        bool focused;
        smath::matrix4x4 VPmatrix;
    };

    ViewportContext createViewportContext(RenderContext* renderContext, const char* name, const ViewportSettings &settings = ViewportSettings());

    void beginViewport(ViewportContext &viewport, Camera &camera);
    void endViewport(ViewportContext &viewport, Camera &camera);
}

#endif