#include "brl_window.hpp"
#include "brl_render.hpp"

#include "brl_builtin_shaders.hpp"

#include <iostream>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");

    brl::Shader shader = brl::createShader(brl::builtin::objectVertexShaderSource, brl::builtin::objectFragShaderSource);

    while (!glfwWindowShouldClose(window->glfwWindow)) {
        brl::updateWindow(window);

        brl::beginRender(window);
            brl::clearRender();

            ImGui::Begin("Stats");
            ImGui::Text("Delta time: %fms", window->deltaTime*1000.0);
            ImGui::Text("Hello World");
            ImGui::End();
            
        brl::endRender();
    }

    brl::destroyWindow(window);
}