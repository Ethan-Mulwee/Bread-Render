#include "brl_viewport.hpp"
#include "brl_framebuffer.hpp"
#include "brl_render.hpp"
#include "imgui.h"

namespace brl {
    ViewportContext createViewportContext(RenderContext *renderContext, const char* name, const ViewportSettings &settings) {
        ViewportContext viewportContext;
        viewportContext.renderContext = renderContext;
        viewportContext.settings = settings;

        viewportContext.renderFramebuffer = createFramebuffer(1080, 1080, settings.MSAA);
        if (settings.ShadowMap) viewportContext.shadowFramebuffer = createFramebuffer(1080, 1080, false);
        if (settings.MSAA) viewportContext.outputFramebuffer = createFramebuffer(1080, 1080, false);

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

        uint64_t textureID;
        brl::resizeFramebuffer(&viewport.renderFramebuffer, viewport.size.x, viewport.size.y);
        if (viewport.settings.MSAA) {
            brl::resizeFramebuffer(&viewport.outputFramebuffer, viewport.size.x, viewport.size.y);
            textureID = viewport.outputFramebuffer.texId;
        } else {
            textureID = viewport.renderFramebuffer.texId;
        }

        ImGui::Image((ImTextureRef)(textureID), viewport.size, ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::SetCursorPos(viewport.position);

        bindFramebuffer(viewport.renderFramebuffer);

    }


    void endViewport(ViewportContext &viewport, Camera &camera) {
        
        renderModeSolid();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);

        useShader(viewport.renderContext->gridShader);
        setShaderUniformMatrix4(viewport.renderContext->gridShader, calculateCameraView(camera), "view");
        setShaderUniformMatrix4(viewport.renderContext->gridShader, calculateCameraProjection(camera), "projection");

        smath::vector3 cameraPosition = calculateCameraPosition(camera);
        const float size = 250.0f;
        smath::matrix4x4 gridTransform = {
            size, 0.0f, 0.0f,                           0.0f,
            0.0f, size, 0.0f,                           0.0f,
            0.0f, 0.0f, size,                           0.0f,
            -cameraPosition.x, 0.0f, -cameraPosition.z, 1.0f
        };
        setShaderUniformMatrix4(viewport.renderContext->gridShader, gridTransform, "model");
        drawVertexBuffer(viewport.renderContext->planeBuffer);

        // There is no need to resolve the framebuffer without MSAA this saves preformance on low end devices
        if (viewport.settings.MSAA)
            brl::blitFramebuffer(viewport.renderFramebuffer.fBO, viewport.outputFramebuffer.fBO, viewport.renderFramebuffer.width, viewport.renderFramebuffer.height);
        unbindFramebuffer();



        ImGui::End();

    }
}