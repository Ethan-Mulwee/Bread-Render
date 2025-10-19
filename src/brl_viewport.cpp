#include "brl_viewport.hpp"

namespace brl {
    ViewportContext createViewportContext(RenderContext *renderContext, int width, int height, const char* name) {
        ViewportContext viewportContext;
        viewportContext.renderContext = renderContext;
        viewportContext.framebuffer = createFramebuffer(width, height);
        viewportContext.hovered = false;
        viewportContext.focused = false;
        viewportContext.name = name;

        return viewportContext;
    }

    void beginViewport(ViewportContext &viewport, Camera &camera) {

        camera.aspect = viewport.size.x / viewport.size.y;

        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        bindFramebuffer(viewport.framebuffer);

        // Set instance uniforms
        brl::useShader(viewport.renderContext->instanceShader);
        setShaderUniformMatrix4(viewport.renderContext->instanceShader, calculateCameraView(camera), "view");
        setShaderUniformMatrix4(viewport.renderContext->instanceShader, calculateCameraProjection(camera), "projection");

        useShader(viewport.renderContext->objectShader);
        setShaderUniformMatrix4(viewport.renderContext->objectShader, calculateCameraView(camera), "view");
        setShaderUniformMatrix4(viewport.renderContext->objectShader, calculateCameraProjection(camera), "projection");

        ImGui::Begin(viewport.name);

        viewport.size = ImGui::GetContentRegionAvail();
        viewport.position = ImGui::GetCursorPos();
        viewport.screenPosition = ImGui::GetCursorScreenPos();
        viewport.hovered = ImGui::IsWindowHovered();
        viewport.focused = ImGui::IsWindowFocused();
        
        uint64_t textureID = viewport.framebuffer.texId;
        ImGui::Image((ImTextureRef)(textureID), viewport.size, ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::SetCursorPos(viewport.position);

    }


    void endViewport(ViewportContext &viewport, Camera &camera) {
        
        renderModeSolid();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);

        // instance rendering
        // brl::useShader(viewport.renderContext->instanceShader);
        // setShaderUniformMatrix4(viewport.renderContext->instanceShader, calculateCameraView(camera), "view");
        // setShaderUniformMatrix4(viewport.renderContext->instanceShader, calculateCameraProjection(camera), "projection");

        // brl::setInstancedVertexBufferData(viewport.renderContext->cubeInstancedVertexBuffer, viewport.renderContext->cubeInstanceBuffer);
        // brl::drawInstancedVertexBuffer(viewport.renderContext->cubeInstancedVertexBuffer, viewport.renderContext->cubeInstanceBuffer.used);
        // brl::clearInstanceDataBuffer(&viewport.renderContext->cubeInstanceBuffer);
        // instance rendering

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

        ImGui::End();

    }
}