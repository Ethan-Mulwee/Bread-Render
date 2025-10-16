#include "brl_window.hpp"
#include "brl_render.hpp"
#include "brl_viewport.hpp"


#include <iostream>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport1 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport1");
    brl::ViewportContext viewport2 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport2");
    brl::Camera camera = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);


    while (!glfwWindowShouldClose(window->glfwWindow)) {
        brl::updateWindow(window);
        brl::updateCamera(&camera, window);

        brl::beginRender(window);
            brl::clearRender();

            ImGui::Begin("Stats");
            ImGui::Text("Delta time: %fms", window->deltaTime*1000.0);
            ImGui::Text("Hello World");
            ImGui::End();

            brl::beginViewport(viewport1, camera);
                brl::drawCube(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
            brl::endViewport(viewport1, camera);

            brl::beginViewport(viewport2, camera);
                brl::drawCone(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 0.1f, 0.0f, 1.0f});
            brl::endViewport(viewport2, camera);
            
        brl::endRender();
    }

    brl::destroyWindow(window);
}