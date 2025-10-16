#include "brl_viewport.hpp"

namespace brl {
    ViewportContext createViewportContext(const RenderContext *renderContext, int width, int height, const char* name) {
        ViewportContext viewportContext;
        viewportContext.renderContext = renderContext;
        viewportContext.framebuffer = createFramebuffer(width, height);
        viewportContext.name = name;

        return viewportContext;
    }

    void beginViewport(const ViewportContext &viewport, const Camera &camera) {

        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        bindFramebuffer(viewport.framebuffer);

        useShader(viewport.renderContext->objectShader);
        setShaderUniformMatrix4(viewport.renderContext->objectShader, calculateCameraView(camera), "view");
        setShaderUniformMatrix4(viewport.renderContext->objectShader, calculateCameraProjection(camera), "projection");

    }

    void endViewport(ViewportContext &viewport, Camera &camera) {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);

        useShader(viewport.renderContext->gridShader);
        setShaderUniformMatrix4(viewport.renderContext->gridShader, calculateCameraView(camera), "view");
        setShaderUniformMatrix4(viewport.renderContext->gridShader, calculateCameraProjection(camera), "projection");

        smath::vector3 cameraPosition = calculateCameraPosition(camera);
        const float size = 250.0f;
        smath::matrix4x4 gridTransform = {
            size, 0.0f, 0.0f, -cameraPosition.x,
            0.0f, size, 0.0f,              0.0f,
            0.0f, 0.0f, size, -cameraPosition.z,
            0.0f, 0.0f, 0.0f,              1.0f
        };
        setShaderUniformMatrix4(viewport.renderContext->gridShader, gridTransform, "model");
        drawVertexBuffer(viewport.renderContext->planeBuffer);

        unbindFramebuffer();

        ImGui::Begin(viewport.name);

        viewport.size = ImGui::GetContentRegionAvail();
        viewport.position = ImGui::GetCursorScreenPos();

        uint64_t textureID = viewport.framebuffer.texId;
        ImGui::Image((ImTextureRef)(textureID), viewport.size, ImVec2{0, 1}, ImVec2{1, 0});

        ImGui::End();

        camera.aspect = viewport.size.x / viewport.size.y;
    }
}