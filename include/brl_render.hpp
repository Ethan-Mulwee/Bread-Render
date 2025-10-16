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

    RenderContext createRenderContext(const Window* window);

    void beginRender(Window* window);
    void clearRender(smath::vector4 color = {0.2f, 0.2f, 0.2f, 0.2f});
    void endRender();

    void renderModeSolid();
    void renderModeWireframe();

    void drawCube(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawCone(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawCylinder(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawSphere(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color  = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawSphere(const RenderContext &context, const smath::vector3 position, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawVector(const RenderContext &context, const smath::vector3 &position, const smath::vector3 &vector, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});


}

#endif