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

    smath::matrix4x4 planeTransform = {
        smath::vector4{5.0f, 0.0f, 0.0f, 0.0f},
        smath::vector4{0.0f, 0.1f, 0.0f, 0.0f},
        smath::vector4{0.0f, 0.0f, 5.0f, 0.0f},
        smath::vector4{0.0f, -1.0f, 0.0f, 1.0f},
    };

    // Create scene data
    brl::SceneData scene;

    brl::Model sphereModel {
        .mesh = {renderContext.sphereBuffer},
        .transform = smath::matrix4x4_from_identity()
    };

    brl::Model planeModel {
        .mesh = {renderContext.cubeBuffer},
        .transform = planeTransform
    };

    brl::Model teapotModel {
        .mesh = utahTeapotMesh,
        .transform = smath::matrix4x4_from_translation({2.0f, -1.0f, 0.0f})*smath::matrix4x4_from_scale(0.4f)
    };

    scene.models.push_back(sphereModel);
    scene.models.push_back(planeModel);
    scene.models.push_back(teapotModel);

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

                brl::drawScene(viewport1, scene);
                // brl::drawSphere(viewport1, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
                // brl::drawMesh(viewport1, utahTeapotMesh, smath::matrix4x4_from_translation({2.0f, -1.0f, 0.0f})*smath::matrix4x4_from_scale(0.4f), smath::vector4{0.0f, 1.0f, 1.0f, 1.0f});
                // brl::drawMesh(viewport1, utahTeapotMesh, smath::matrix4x4_from_translation({2.0f, -1.0f, 2.0f})*smath::matrix4x4_from_scale(0.4f), smath::vector4{0.0f, 1.0f, 1.0f, 1.0f});
                // brl::drawCube(viewport1, planeTransform, smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
                // // TODO: fix shadow bug
                // brl::drawMesh(viewport1, utahTeapotMesh, smath::matrix4x4_from_translation({-2.0f, -1.0f, 0.0f})*smath::matrix4x4_from_scale(0.4f), smath::vector4{0.0f, 1.0f, 1.0f, 1.0f});

            brl::endViewport(viewport1, camera1);

            brl::beginViewport(viewport2, camera2);

                ImGui::Text("This is text dispalyed ontop of the viewport!");
                ImGui::Text("size: %f, %f", viewport2.size.x, viewport2.size.y);
            
                brl::drawMesh(viewport2, utahTeapotMesh, smath::matrix4x4_from_scale(0.4f), smath::vector4{1.0f, 0.1f, 0.0f, 1.0f});

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