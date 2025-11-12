#ifndef BRL_RENDER
#define BRL_RENDER

#include "brl_window.hpp"
#include "brl_vertexbuffer.hpp"
#include "brl_shader.hpp"
#include "brl_commandbuffer.hpp"

namespace brl {
    struct RenderContext {
        
        const Window* window;

        Shader objectShader;
        Shader gridShader;
        Shader instanceShader;

        Vertexbuffer sphereBuffer;
        Vertexbuffer cylinderBuffer;
        Vertexbuffer coneBuffer;
        Vertexbuffer cubeBuffer;
        Vertexbuffer planeBuffer;
        
        RenderGrouper batcher;
    };

    RenderContext createRenderContext(const Window* window);
    void destroyRenderContext(RenderContext* context);

    void beginRender(Window* window);
    void clearRender(smath::vector4 color = {0.2f, 0.2f, 0.2f, 0.2f});
    void endRender();

    void renderModeSolid();
    void renderModeWireframe();
    void renderModeTransparent();

    void drawCube(RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawCubes(RenderContext &context, ModelData* data, const uint32_t count);

    void drawCone(RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawCones(RenderContext &context, ModelData* data, const uint32_t count);

    void drawCylinder(RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawCylinders(RenderContext &context, ModelData* data, const uint32_t count);

    void drawSphere(RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color  = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawSphere(RenderContext &context, const smath::vector3 position, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawSpheres(RenderContext &context, ModelData* data, const uint32_t count);

    void drawPlane(RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawPlanes(RenderContext &context, ModelData* data, const uint32_t count);

    void drawVector(RenderContext &context, const smath::vector3 &position, const smath::vector3 &vector, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});

    void drawMesh(RenderContext &context, const Mesh &mesh, const smath::matrix4x4 &transform, const smath::vector4 &color);
    void drawMeshes(RenderContext &context, const Mesh &mesh, ModelData* data, const uint32_t count);
    
    void drawModel(RenderContext &context, const Model &model);
    void drawModels(RenderContext &context, const Model &model, ModelData* data, const uint32_t count);


}

#endif