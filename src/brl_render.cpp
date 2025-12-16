#include "brl_render.hpp"
#include "brl_builtin_mesh.hpp"
#include "brl_builtin_shaders.hpp"
#include "brl_shader.hpp"
#include "brl_vertexbuffer.hpp"

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

    void clearRender(const Color &color) {
        glClearColor(color.vector.x, color.vector.y, color.vector.z, color.vector.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void endRender() {
        endImGuiRender();
    }

    void renderModeSolid() {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_CULL_FACE);   
        glDisable(GL_BLEND);
    }

    void renderModeWireframe(float line_width) {
        glLineWidth(line_width);
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

        context.unlitShader = createShader(
            brl::builtin::objectVertexShaderSource,
            brl::builtin::unlitFragShaderSource
        );

        context.gridShader = createShader(
            brl::builtin::objectVertexShaderSource, 
            brl::builtin::gridFragShaderSource
        );

        context.instanceShader = createShader(
            brl::builtin::instancedObjectVertexShaderSource,
            brl::builtin::instancedObjectFragShaderSource
        );

        return context;
    }

    void destroyRenderContext(RenderContext* context) {

    }

    void useShader(const Shader &shader) {
        glUseProgram(shader.programId);
    }

    /* ---------------------------------- Cube ---------------------------------- */

    void drawCube(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.cubeBuffer);
    }

    void drawCubeInstances(const RenderContext &context, const InstanceData* data, const uint32_t count) {
        brl::useShader(context.instanceShader);
        brl::drawVertexbufferInstanced(context.cubeBuffer, data, count);
        brl::useShader(context.objectShader);
    }

    /* ---------------------------------- Cone ---------------------------------- */

    void drawCone(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.coneBuffer);
    }

    void drawConeInstances(const RenderContext &context, const InstanceData *data, uint32_t count) {
        brl::useShader(context.instanceShader);
        brl::drawVertexbufferInstanced(context.coneBuffer, data, count);
        brl::useShader(context.objectShader);
    }

    /* -------------------------------- Cylinder -------------------------------- */

    void drawCylinder(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.cylinderBuffer);
    }

    void drawCylinderInstances(const RenderContext &context, const InstanceData *data, uint32_t count) {
        brl::useShader(context.instanceShader);
        brl::drawVertexbufferInstanced(context.cylinderBuffer, data, count);
        brl::useShader(context.objectShader);
    }

    /* --------------------------------- Sphere --------------------------------- */


    void drawSphere(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.sphereBuffer);
    }

    void drawSphereInstances(const RenderContext &context, const InstanceData *data, uint32_t count) {
        brl::useShader(context.instanceShader);
        brl::drawVertexbufferInstanced(context.sphereBuffer, data, count); 
        brl::useShader(context.objectShader);
    }

    void drawSphere(const RenderContext &context, const smath::vector3 position, const float radius, const smath::vector4 &color) {
        smath::vector4 i = {radius, 0.0f, 0.0f, 0.0f};
        smath::vector4 j = {0.0f, radius, 0.0f, 0.0f};
        smath::vector4 k = {0.0f, 0.0f, radius, 0.0f};
        smath::vector4 l = {position.x, position.y, position.z, 1.0f};
        smath::matrix4x4 transform = {i, j, k, l};
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.sphereBuffer);
    }

    /* --------------------------------- Circle --------------------------------- */

    // Code adapted from raylib
    void draw_arc(const RenderContext &context, const smath::matrix4x4 &transform, const float radius, float start_angle, float end_angle, uint segments, const Color &color) {
        const float DEG2RAD = M_PI/180.0f;

        glDisable(GL_CULL_FACE);
        useShader(context.unlitShader);
        setShaderUniformMatrix4(context.unlitShader, transform, "model");
        setShaderUniformFloat4(context.unlitShader, color.vector, "color");

        if (end_angle < start_angle) {
            float tmp = start_angle;
            start_angle = end_angle;
            end_angle = tmp;
        }

        int min_segments = (int)ceilf((end_angle - start_angle)/90.0f);
        
        if (segments < min_segments) {
            // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
            float th = acosf(2*powf(1 - 0.5f/radius, 2) - 1);
            segments = (int)((end_angle - start_angle)*ceilf(2*M_PI/th)/360);
    
            if (segments <= 0) segments = min_segments;
        }

        float stepLength = (end_angle - start_angle)/(float)segments;
        float angle = start_angle;

        glBegin(GL_TRIANGLES);
            for (int i = 0; i < segments; i++)
            {
                glVertex3f(0, 0, 0);
                glVertex3f(cosf(DEG2RAD*(angle + stepLength))*radius, sinf(DEG2RAD*(angle + stepLength))*radius, 0);
                glVertex3f(cosf(DEG2RAD*angle)*radius, sinf(DEG2RAD*angle)*radius, 0);

                angle += stepLength;
            }
        glEnd();
    }

    void draw_circle_lines(const RenderContext &context, const smath::matrix4x4 &transform, const float radius, float line_width, const Color &color) {

        using namespace smath;

        matrix4x4 transformation_matrix = transform;

        useShader(context.unlitShader);

        glLineWidth((GLfloat) line_width);
        setShaderUniformFloat4(context.unlitShader, color.vector, "color");
        setShaderUniformMatrix4(context.unlitShader, transformation_matrix, "model");

        glBegin(GL_LINES);
            for (int i = 0; i < 180; i++) {
                float angle = ((float)i / 180.0f) * M_PI*2.0f;
                float inc_angle = (1.0f / 180.0f) * M_PI * 2.0f;
                smath::vector3 a = {(float)cos(angle) * radius, (float)sin(angle) * radius, 0.0f};
                glVertex3f(a.x, a.y, a.z);


                smath::vector3 b = {(float)cos(angle + inc_angle) * radius, (float)sin(angle + inc_angle) * radius, 0.0f};
                glVertex3f(b.x, b.y, b.z);
            }
        glEnd();

    }


    void draw_circle_lines(const RenderContext &context, const smath::vector3 &position, const smath::vector3 &direction, const float radius, float line_width, const Color &color) {

        using namespace smath;

        quaternion rotation = normalize(quaternion_from_matrix3x3(matrix3x3_from_khat(normalize(direction))));

        transform transform_struct = {
            .translation = position,
            .rotation = rotation,
            .scale = {1, 1, 1}
        };
        matrix4x4 transformation_matrix = smath::matrix4x4_from_transform(transform_struct);
        
        useShader(context.unlitShader);

        glLineWidth((GLfloat) line_width);
        setShaderUniformFloat4(context.unlitShader, color.vector, "color");
        setShaderUniformMatrix4(context.unlitShader, transformation_matrix, "model");

        glBegin(GL_LINES);
            for (int i = 0; i < 180; i++) {
                float angle = ((float)i / 180.0f) * M_PI*2.0f;
                float inc_angle = (1.0f / 180.0f) * M_PI * 2.0f;
                smath::vector3 a = {(float)cos(angle) * radius, (float)sin(angle) * radius, 0.0f};
                glVertex3f(a.x, a.y, a.z);


                smath::vector3 b = {(float)cos(angle + inc_angle) * radius, (float)sin(angle + inc_angle) * radius, 0.0f};
                glVertex3f(b.x, b.y, b.z);
            }
        glEnd();

    }


    /* ---------------------------------- Plane --------------------------------- */

    void drawPlane(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color) {
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color, "color");
        drawVertexBuffer(context.sphereBuffer);
    }

    void drawPlaneInstances(const RenderContext &context, const InstanceData *data, uint32_t count) {
        brl::useShader(context.instanceShader);
        brl::drawVertexbufferInstanced(context.planeBuffer, data, count); 
        brl::useShader(context.objectShader);
    }

    /* --------------------------------- Vector --------------------------------- */

    void drawVector(const RenderContext &context, const smath::vector3 &position, const smath::vector3 &vector, const float radius, const smath::vector4 &color) {

        using namespace smath;

        vector3 direction = normalize(vector);
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

    /* ---------------------------------- Line ---------------------------------- */

    void drawLine(const RenderContext &context, const smath::vector3 &position1, const smath::vector3 &position2, const float radius, const smath::vector4 &color) {
        
        using namespace smath;

        vector3 vector = (position2 - position1);
        vector3 direction = normalize(vector);
        quaternion orientation = quaternion_from_matrix3x3(matrix3x3_from_jhat(direction));

        matrix4x4 translation = matrix4x4_from_translation({0.0f, 1.0f, 0.0f});

        transform cylinderTransform {
            .translation = position1,
            .rotation = orientation,
            .scale = {radius, vector.length()*0.5f, radius}
        };

        drawCylinder(context, matrix4x4_from_transform(cylinderTransform)*translation, color);
    }

    /* ---------------------------------- Mesh ---------------------------------- */

    void drawMesh(const RenderContext &context, const Mesh &mesh, const smath::matrix4x4 &transform, const Color &color) {
        useShader(context.objectShader);
        setShaderUniformMatrix4(context.objectShader, transform, "model");
        setShaderUniformFloat4(context.objectShader, color.vector, "color");
        drawVertexBuffer(mesh.buffer);
    }

    void drawMeshInstances(const RenderContext &context, const Mesh &mesh, const InstanceData *data, const uint32_t count) {
        useShader(context.instanceShader);
        drawVertexbufferInstanced(mesh.buffer, data, count);
        useShader(context.objectShader);

    }

    void drawModel(const RenderContext &context, const Model &model) {
        setShaderUniformMatrix4(context.objectShader, model.transform, "model");
        setShaderUniformFloat4(context.objectShader, model.color, "color");
        drawVertexBuffer(model.mesh.buffer);
    }
    
}