#ifndef BRL_RENDER
#define BRL_RENDER

#include "brl_window.hpp"
#include "brl_scene.hpp"
#include "brl_shader.hpp"

namespace brl {
    struct ViewportContext;

    struct RenderContext {
        
        const Window* window;

        Shader objectShader;
        Shader gridShader;
        Shader instanceShader;
        Shader shadowShader;

        Vertexbuffer sphereBuffer;
        Vertexbuffer cylinderBuffer;
        Vertexbuffer coneBuffer;
        Vertexbuffer cubeBuffer;
        Vertexbuffer planeBuffer;
        
    };

    RenderContext createRenderContext(const Window* window);
    void destroyRenderContext(RenderContext* context);

    void beginRender(Window* window);
    void clearRender(smath::vector4 color = {0.2f, 0.2f, 0.2f, 0.2f});
    void endRender();

    void renderModeSolid();
    void renderModeWireframe();
    void renderModeTransparent();

    void drawCube(const ViewportContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawCubeInstances(const RenderContext &context, const InstanceData* data, const uint32_t count);

    void drawCone(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawConeInstances(const RenderContext &context, const InstanceData* data, uint32_t count);

    void drawCylinder(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawCylinderInstances(const RenderContext &context, const InstanceData* data, uint32_t count);

    void drawSphere(const ViewportContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color  = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawSphere(const RenderContext &context, const smath::vector3 position, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawSphereInstances(const RenderContext &context, const InstanceData* data, uint32_t count);

    void drawPlane(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawPlaneInstances(const RenderContext &context, const InstanceData* data, uint32_t count);

    void drawVector(const RenderContext &context, const smath::vector3 &position, const smath::vector3 &vector, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});

    void drawMesh(const ViewportContext &context, const Mesh &mesh, const smath::matrix4x4 &transform, const smath::vector4 &color);
    void drawMeshInstances(const RenderContext &context, const Mesh &mesh, const InstanceData* data, const uint32_t amount);
    
    void drawModel(const RenderContext &context, const Model &model);

    void drawScene(const ViewportContext &context, const SceneData &data);


}

#endif