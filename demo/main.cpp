#include "brl_window.hpp"
#include "brl_render.hpp"
#include "brl_viewport.hpp"


#include <iostream>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport = brl::createViewportContext(&renderContext, 1920, 1080, "viewport");
    brl::Camera camera = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);


    while (!glfwWindowShouldClose(window->glfwWindow)) {
        brl::updateWindow(window);

        brl::beginRender(window);
            brl::clearRender();

            ImGui::Begin("Stats");
            ImGui::Text("Delta time: %fms", window->deltaTime*1000.0);
            ImGui::Text("Hello World");
            ImGui::End();

            brl::beginViewport(viewport, camera);
            brl::endViewport(viewport, camera);
            
        brl::endRender();
    }

    brl::destroyWindow(window);
}