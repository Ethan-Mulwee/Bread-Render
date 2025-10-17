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

    void renderModeSolid() {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_CULL_FACE);   
        glDisable(GL_BLEND);
    }

    void renderModeWireframe() {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

    void renderModeTransparent() {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

        return context;
    }

    void useShader(const Shader &shader) {
        glUseProgram(shader.programId);
    }

    void drawCube(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.cubeBuffer);
    }

    void drawCone(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.coneBuffer);
    }

    void drawCylinder(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.cylinderBuffer);
    }

    void drawSphere(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.sphereBuffer);
    }

    void drawSphere(const RenderContext &context, const smath::vector3 position, const float radius, const smath::vector4 &color) {
        smath::matrix4x4 transform = {
            radius, 0.0f, 0.0f, position.x,
            0.0f, radius, 0.0f, position.y,
            0.0f, 0.0f, radius, position.z,
            0.0f, 0.0f,   0.0f,       1.0f,
        };
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.sphereBuffer);
    }

    void drawVector(const RenderContext &context, const smath::vector3 &position, const smath::vector3 &vector, const float radius, const smath::vector4 &color) {

        using namespace smath;

        vector3 direction = normalized(vector);
        quaternion orientation = quaternion_from_matrix3x3(matrix3x3_from_jhat(direction));

        matrix4x4 translation = matrix4x4_from_translation({0.0f, 1.0f, 0.0f});

        transform cylinderTransform {
            .translation = position,
            .rotation = orientation,
            .scale = {radius, vector.length()*0.55f, radius}
        };

        transform coneTransform {
            .translation = position + vector,
            .rotation = orientation,
            .scale = {radius * 2.0f, radius * 2.0f, radius * 2.0f}
        };
        
        drawCylinder(context, matrix4x4_from_transform(cylinderTransform)*translation, color);
        drawCone(context, matrix4x4_from_transform(coneTransform)*translation, color);
    }

    void drawMesh(const RenderContext &context, const Mesh &mesh, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(mesh.buffer);
    }

    void drawModel(const RenderContext &context, const Model &model) {
        setShaderUniformMatrix4(context.objectShader, model.transform, "model");
        setShaderUniformFloat4(context.objectShader, model.color, "color");
        drawVertexBuffer(model.mesh.buffer);
    }
}