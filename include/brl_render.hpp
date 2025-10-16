#ifndef BRL_RENDER
#define BRL_RENDER

#include "brl_window.hpp"
#include "brl_framebuffer.hpp"
#include "brl_vertexbuffer.hpp"
#include "brl_shader.hpp"

namespace brl {

    struct RenderContext {
        const Window* window;

        Shader objectShader;
        Shader gridShader;

        Vertexbuffer sphereBuffer;
        Vertexbuffer cylinderBuffer;
        Vertexbuffer coneBuffer;
        Vertexbuffer cubeBuffer;
        Vertexbuffer planeBuffer;
    };

    void beginRender(Window* window);
    void clearRender(smath::vector4 color = {0.2f, 0.2f, 0.2f, 0.2f});
    void endRender();

    void bindRenderContext(RenderContext context);

    RenderContext createRenderContext(const Window* window);

    void drawCube(const RenderContext &context);
    void drawCone(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color);
    void drawCylinder(const RenderContext &context);
    void drawSphere(const RenderContext &context);
}

#endif