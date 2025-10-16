#include "brl_window.hpp"
#include "brl_render.hpp"
#include "brl_viewport.hpp"

#include <cstdlib>
#include <iostream>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    // brl::setVsync(false);
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport1 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport1");
    brl::ViewportContext viewport2 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport2");
    brl::Camera camera1 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    // const int cubeAmount = 100000;
    // smath::matrix4x4* cubeTransforms = new smath::matrix4x4[cubeAmount];
    // for (int i = 0; i < cubeAmount; i++) {
    //     int positionIntX = rand() % 100000;
    //     int positionIntY = rand() % 100000;
    //     int positionIntZ = rand() % 100000;

    //     float positionX = positionIntX * 0.0001f;
    //     float positionY = positionIntY * 0.0001f;
    //     float positionZ = positionIntZ * 0.0001f;

    //     cubeTransforms[i] = smath::matrix4x4_from_translation({positionX, positionY, positionZ}) * smath::matrix4x4_from_scale(0.1f);
    // }


    while (!glfwWindowShouldClose(window->glfwWindow)) {
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
                brl::drawCube(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
                // for (int i = 0; i < cubeAmount; i++) {
                //     brl::drawCube(renderContext, cubeTransforms[i]);
                // }
            brl::endViewport(viewport1, camera1);

            brl::beginViewport(viewport2, camera2);

                ImGui::Text("This is text dispalyed ontop of the viewport!");
            
                brl::drawCone(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 0.1f, 0.0f, 1.0f});

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

    brl::destroyWindow(window);
}