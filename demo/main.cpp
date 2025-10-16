#include "brl_window.hpp"
#include "brl_render.hpp"
#include "brl_viewport.hpp"


#include <iostream>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport1 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport1");
    brl::ViewportContext viewport2 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport2");
    brl::Camera camera1 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);


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
            brl::endViewport(viewport1, camera1);

            brl::beginViewport(viewport2, camera2);
            
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