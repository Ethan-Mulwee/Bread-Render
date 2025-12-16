#ifndef BRENDER_WINDOW
#define BRENDER_WINDOW

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "smath.hpp"

namespace brl {
    struct Window {
        GLFWwindow* glfwWindow;
        int width, height;
        const char* name;
        
        smath::vector2 mousePos = {0.0f,0.0f};
        smath::vector2 deltaMousePos = {0.0f,0.0f};
        float scrollInput = 0.0f;
        float time = 0.0f;
        float deltaTime = 0.0f;
        double perviousRenderTime = 0.0;
    };
    
    Window* create_window(int width, int height, const char* name);

    void update_window(Window* window);

    void destroy_window(Window* window);

    bool window_should_close(const Window* window);

    void set_vsync(bool boolean);
}

#endif