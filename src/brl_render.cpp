#include "brl_render.hpp"
#include "brl_builtin_mesh.hpp"
#include "brl_builtin_shaders.hpp"

namespace brl {
    void beginImGuiRender() {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Create the docking environment
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;
        
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
        ImGui::PopStyleVar(3);
        
        ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
        
        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
    }

    void endImGuiRender() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            // ImGui::UpdatePlatformWindows();
            // ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void beginRender(Window* window) {
        glViewport(0, 0, window->width, window->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        beginImGuiRender();
    }

    void clearRender(smath::vector4 color) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    }

    void endRender() {
        endImGuiRender();
    }

    RenderContext createRenderContext(const Window *window) {
        RenderContext context;

        context.window = window;

        context.cubeBuffer = createVertexbuffer(&brl::builtin::cubeMesh);
        context.coneBuffer = createVertexbuffer(&brl::builtin::coneMesh);
        context.cylinderBuffer = createVertexbuffer(&brl::builtin::cylinderMesh);
        context.sphereBuffer = createVertexbuffer(&brl::builtin::sphereMesh);
        context.planeBuffer = createVertexbuffer(&brl::builtin::planeMesh);

        context.objectShader = createShader(
            brl::builtin::objectVertexShaderSource,
            brl::builtin::objectFragShaderSource
        );

        context.gridShader = createShader(
            brl::builtin::objectVertexShaderSource, 
            brl::builtin::gridFragShaderSource
        );
    }
}