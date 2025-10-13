#include "brl_window.hpp"

#include <iostream>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");

    while (!glfwWindowShouldClose(window->glfwWindow)) {
        brl::updateWindow(window);

        
    }

    brl::destroyWindow(window);
}