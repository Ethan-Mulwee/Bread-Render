#include "brl.hpp"

#include <cstdlib>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::setVsync(false);
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportSettings viewportSettings = {.shadowMap = true, .MSAA = 4};
    brl::ViewportContext viewport1 = brl::createViewportContext(&renderContext, "viewport1", viewportSettings);
    brl::ViewportContext viewport2 = brl::createViewportContext(&renderContext, "viewport2", viewportSettings);
    brl::Camera camera1 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    brl::MeshData utahTeapotMeshData = brl::parseObj("../demo/OBJs/Utah-Teapot.obj");
    brl::Mesh utahTeapotMesh = brl::createMesh(&utahTeapotMeshData);
    brl::MeshData cubeMeshData = brl::parseObj("../demo/OBJs/Primitive-Cube.obj");

    brl::InstanceData* instanceData = new brl::InstanceData[100000];
    for (int i = 0; i < 100000; i++) {
        int positionIntX = rand() % 100000;
        int positionIntY = rand() % 100000;
        int positionIntZ = rand() % 100000;

        float positionX = positionIntX * 0.0001f;
        float positionY = positionIntY * 0.0001f;
        float positionZ = positionIntZ * 0.0001f;

        smath::matrix4x4 transform = {
            .i = {0.015f, 0.0f, 0.0f, 0.0f},
            .j = {0.0f, 0.015f, 0.0f, 0.0f},
            .k = {0.0f, 0.0f, 0.015f, 0.0f},
            .l = {positionX, positionY, positionZ, 1.0f},
        };
        instanceData[i] = {transform, smath::vector4{1.0f, 1.0f, 1.0f, 1.0f}};
    }

    while (!brl::windowShouldClose(window)) {
        brl::updateWindow(window);
        if (viewport1.hovered) brl::updateCamera(&camera1, window);
        if (viewport2.hovered) brl::updateCamera(&camera2, window);

        brl::beginRender(window);
            brl::clearRender();

            ImGui::Begin("Stats");
            ImGui::Text("Delta time: %fms", window->deltaTime*1000.0);
            ImGui::Text("Hello World");
            ImGui::End();

            ImGui::Begin("Shadow-Map");
            ImVec2 imageSize = ImGui::GetContentRegionAvail();
            ImGui::Image((ImTextureRef)(viewport1.shadowFramebuffer.depthId), imageSize, ImVec2{0, 1}, ImVec2{1, 0});
            ImGui::End();

            brl::beginViewport(viewport1, camera1);
                smath::matrix4x4 planeTransform = {
                    smath::vector4{5.0f, 0.0f, 0.0f, 0.0f},
                    smath::vector4{0.0f, 0.1f, 0.0f, 0.0f},
                    smath::vector4{0.0f, 0.0f, 5.0f, 0.0f},
                    smath::vector4{0.0f, -1.0f, 0.0f, 1.0f},
                };

                smath::vector3 lightInvDir = normalize(smath::vector3{1.0,2.0,-0.4});

                smath::matrix4x4 depthProjectionMatrix = smath::matrix4x4_from_orthographic(-10, 10, -10, 10, -10, 20);
                smath::matrix4x4 depthViewMatrix = smath::matrix4x4_from_look(lightInvDir, {0,0,0}, {0,1,0});
                smath::matrix4x4 depthModelMatrix = smath::matrix4x4_from_identity();
                smath::matrix4x4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

                glCullFace(GL_FRONT);
                brl::bindFramebuffer(viewport1.shadowFramebuffer);
                brl::clearFramebuffer();
                brl::useShader(renderContext.shadowShader);
                brl::setShaderUniformMatrix4(renderContext.shadowShader, depthMVP, "depthMVP");
                brl::drawSphere(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f}); 
                brl::setShaderUniformMatrix4(renderContext.shadowShader, depthProjectionMatrix * depthViewMatrix * planeTransform, "depthMVP");
                brl::drawCube(renderContext, planeTransform, smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
                brl::bindFramebuffer(viewport1.renderFramebuffer);
                brl::useShader(renderContext.objectShader);
                glCullFace(GL_BACK);


                smath::matrix4x4 biasMatrix{
                    0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    0.5, 0.5, 0.5, 1.0
                };

                glActiveTexture(GL_TEXTURE0);
                
                glBindTexture(GL_TEXTURE_2D, viewport1.shadowFramebuffer.depthId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                brl::setShaderUniformMatrix4(renderContext.objectShader, biasMatrix*depthMVP, "depthBiasMVP");
                brl::drawSphere(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
                brl::setShaderUniformMatrix4(renderContext.objectShader, biasMatrix * depthProjectionMatrix * depthViewMatrix * planeTransform, "depthBiasMVP");
                brl::drawCube(renderContext, planeTransform, smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
                glBindTexture(GL_TEXTURE_2D, 0);


            brl::endViewport(viewport1, camera1);

            brl::beginViewport(viewport2, camera2);

                ImGui::Text("This is text dispalyed ontop of the viewport!");
                ImGui::Text("size: %f, %f", viewport2.size.x, viewport2.size.y);
            
                brl::drawMesh(renderContext, utahTeapotMesh, smath::matrix4x4_from_scale(0.4f), smath::vector4{1.0f, 0.1f, 0.0f, 1.0f});

                brl::renderModeWireframe();
                brl::drawSphere(renderContext, smath::vector3{2.2f, 0.0f, 0.0f}, 1.0f, smath::vector4{0.0f, 0.9f, 0.1f});
                brl::drawCylinder(renderContext, smath::matrix4x4_from_translation(smath::vector3{-2.2f, 0.0f, 0.0f}));

                brl::renderModeSolid();
                brl::drawVector(renderContext, {0.0f, 0.0f, 2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f);

                brl::renderModeTransparent();
                brl::drawVector(renderContext, {0.0f, 0.0f, -2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});

            brl::endViewport(viewport2, camera2);
            
        brl::endRender();
    }

    delete [] instanceData;
    brl::destroyRenderContext(&renderContext);
    brl::destroyWindow(window);
}