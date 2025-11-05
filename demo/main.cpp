#include "brl.hpp"

#include <cstdlib>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::setVsync(false);
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport1 = brl::createViewportContext(&renderContext, "viewport1", 4);
    brl::ViewportContext viewport2 = brl::createViewportContext(&renderContext, "viewport2", 4);
    brl::Camera camera1 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    brl::MeshData utahTeapotMeshData = brl::parseObj("../demo/OBJs/Utah-Teapot.obj");
    brl::Mesh utahTeapotMesh = brl::createMesh(&utahTeapotMeshData);
    brl::MeshData cubeMeshData = brl::parseObj("../demo/OBJs/Primitive-Cube.obj");

    brl::ModelData* instanceData = new brl::ModelData[500000];
    for (int i = 0; i < 500000; i++) {
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

            brl::beginViewport(viewport1, camera1);
                for (int i = 0; i < 500000; i++)
                    brl::drawCube(renderContext, instanceData[i].transform, smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
                // brl::drawCubes(renderContext, instanceData, 500000);
            brl::endViewport(viewport1, camera1);

            brl::beginViewport(viewport2, camera2);

                // ImGui::Text("This is text dispalyed ontop of the viewport!");
                // ImGui::Text("size: %f, %f", viewport2.size.x, viewport2.size.y);
            
                // brl::drawMesh(renderContext, utahTeapotMesh, smath::matrix4x4_from_scale(0.4f), smath::vector4{1.0f, 0.1f, 0.0f, 1.0f});

                // brl::renderModeWireframe();
                // brl::drawSphere(renderContext, smath::vector3{2.2f, 0.0f, 0.0f}, 1.0f, smath::vector4{0.0f, 0.9f, 0.1f});
                brl::drawCylinder(renderContext, smath::matrix4x4_from_translation(smath::vector3{-2.2f, 0.0f, 0.0f}));
                brl::drawCube(renderContext, smath::matrix4x4_from_translation(smath::vector3{-2.2f, 0.0f, 0.0f}));

                // brl::renderModeSolid();
                // brl::drawVector(renderContext, {0.0f, 0.0f, 2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f);

                // // brl::renderModeTransparent();
                // brl::drawVector(renderContext, {0.0f, 0.0f, -2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});

            brl::endViewport(viewport2, camera2);
            
        brl::endRender();
    }

    delete [] instanceData;
    brl::destroyRenderContext(&renderContext);
    brl::destroyWindow(window);
}