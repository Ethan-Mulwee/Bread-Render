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
        if (settings.shadowMap) viewportContext.shadowFramebuffer = createFramebuffer(2096, 2096, false);
        if (settings.MSAA) viewportContext.outputFramebuffer = createFramebuffer(1080, 1080, false);

        viewportContext.renderCommandBuffer = createRenderCommandBuffer(512);

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
        
        brl::useShader(viewport.renderContext->shadowShader);
        setShaderUniformMatrix4(viewport.renderContext->shadowShader, smath::matrix4x4_from_look(normalize(smath::vector3{1.0,2.0,-0.4}), {0,0,0}, {0,1,0}), "view");
        setShaderUniformMatrix4(viewport.renderContext->shadowShader, smath::matrix4x4_from_orthographic(-10, 10, -10, 10, -10, 20), "projection");
        viewport.VPmatrix = smath::matrix4x4_from_orthographic(-10, 10, -10, 10, -10, 20) * smath::matrix4x4_from_look(normalize(smath::vector3{1.0,2.0,-0.4}), {0,0,0}, {0,1,0});

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

        brl::bindFramebuffer(viewport.shadowFramebuffer);
        brl::clearFramebuffer();

        bindFramebuffer(viewport.renderFramebuffer);
        clearFramebuffer();

    }


    void endViewport(ViewportContext &viewport, Camera &camera) {
        
        renderModeSolid();

        /* -------------------------- Draw Render Commands -------------------------- */

        // Shadow Pass
        glCullFace(GL_FRONT);
        useShader(viewport.renderContext->shadowShader);
        bindFramebuffer(viewport.shadowFramebuffer);

        glBindTexture(GL_TEXTURE_2D, viewport.shadowFramebuffer.depthId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glActiveTexture(GL_TEXTURE0);

        for (int i = 0; i < viewport.renderCommandBuffer.modelBufferUsed; i++) {
            const Model& model = viewport.renderCommandBuffer.modelBuffer[i];
            setShaderUniformMatrix4(viewport.renderContext->shadowShader, model.transform, "model");
            drawVertexBuffer(model.mesh.buffer);
        }

        for (int i = 0; i < viewport.renderCommandBuffer.sceneBufferUsed; i++) {
            const SceneData* scene = viewport.renderCommandBuffer.sceneDataBuffer[i];
            for (int j = 0; j < scene->models.size(); j++) {
                const Model& model = scene->models[j];
                setShaderUniformMatrix4(viewport.renderContext->shadowShader, model.transform, "model");
                drawVertexBuffer(model.mesh.buffer);
            }
        }

        // Render Pass
        glCullFace(GL_BACK);
        useShader(viewport.renderContext->objectShader);
        smath::matrix4x4 biasMatrix{
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        };
        bindFramebuffer(viewport.renderFramebuffer);
        
        for (int i = 0; i < viewport.renderCommandBuffer.modelBufferUsed; i++) {
            const Model& model = viewport.renderCommandBuffer.modelBuffer[i];
            smath::matrix4x4 depthMVP = viewport.VPmatrix * model.transform;
            setShaderUniformFloat4(viewport.renderContext->objectShader, model.color, "color");
            setShaderUniformMatrix4(viewport.renderContext->objectShader, model.transform, "model");
            brl::setShaderUniformMatrix4(viewport.renderContext->objectShader, biasMatrix*depthMVP, "depthBiasMVP");
            drawVertexBuffer(model.mesh.buffer);
        }

        for (int i = 0; i < viewport.renderCommandBuffer.sceneBufferUsed; i++) {
            const SceneData* scene = viewport.renderCommandBuffer.sceneDataBuffer[i];
            for (int j = 0; j < scene->models.size(); j++) {
                const Model& model = scene->models[j];
                smath::matrix4x4 depthMVP = viewport.VPmatrix * model.transform;
                setShaderUniformFloat4(viewport.renderContext->objectShader, model.color, "color");
                setShaderUniformMatrix4(viewport.renderContext->objectShader, model.transform, "model");
                brl::setShaderUniformMatrix4(viewport.renderContext->objectShader, biasMatrix*depthMVP, "depthBiasMVP");
                drawVertexBuffer(model.mesh.buffer);
            }
        }

        // Draw Grid
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