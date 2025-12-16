#include "brl_window.hpp"

#include <iostream>
#include <cstdio>

namespace brl {

    void imGuiInit(GLFWwindow *window) {
        const char* glsl_version = "#version 130";

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        // colors from jayanam
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };

        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void openGLInit() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_MULTISAMPLE); 
    }


    Window* create_window(int width, int height, const char *name) {
        Window *window = new Window();

        window->width = width;
        window->height = height;
        window->name = name;

        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        if (!glfwInit()) {
            fprintf(stderr, "Failed to intialize GLFW\n");
            exit(EXIT_FAILURE);
        }

        window->glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);

        if (!window->glfwWindow) {
            fprintf(stderr, "Failed to open GLFW window \n");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window->glfwWindow);
        glfwSwapInterval(1); // Enable vsync

        // glfwSetScrollCallback(glfwWindow, onScrollCallback);

        glewInit();

        std::cout << glGetString(GL_VERSION) << std::endl;
        std::cout << glGetString(GL_RENDERER) << std::endl;

        glfwSetWindowUserPointer(window->glfwWindow, window);
        glfwSetScrollCallback(window->glfwWindow, [](GLFWwindow *window,
                                                double xoffset, double yoffset) {
            Window *windowA = (Window *)glfwGetWindowUserPointer(window);
            windowA->scrollInput = yoffset;
        });

        imGuiInit(window->glfwWindow);
        openGLInit();

        return window; 
    }

    void update_window(Window *window) {
        window->scrollInput = 0.0f;

        glfwSwapBuffers(window->glfwWindow);
        glfwPollEvents();

        double x, y;
        glfwGetCursorPos(window->glfwWindow, &x, &y);
        smath::vector2 currentPos = smath::vector2{(float)x, (float)y};
        window->deltaMousePos = currentPos - window->mousePos;
        window->mousePos = currentPos;

        float currentTime = glfwGetTime();
        window->deltaTime = currentTime - window->time;
        window->time = currentTime;
    }

    void destroy_window(Window *window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window->glfwWindow);
        delete window;
        glfwTerminate();
    }

    void set_vsync(bool boolean) {
        glfwSwapInterval(boolean); // Enable vsync
    }

    bool window_should_close(const Window *window) {
        return glfwWindowShouldClose(window->glfwWindow);
    }
}