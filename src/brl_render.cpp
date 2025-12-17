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

    void begin_render(Window* window) {
        glViewport(0, 0, window->width, window->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        beginImGuiRender();
    }

    void clear_render(const Color &color) {
        glClearColor(color.vector.x, color.vector.y, color.vector.z, color.vector.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void end_render() {
        endImGuiRender();
    }

    void render_mode_solid() {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_CULL_FACE);   
        glDisable(GL_BLEND);
    }

    void render_mode_wireframe(float line_width) {
        glLineWidth(line_width);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

    void render_mode_transparent() {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    RenderContext create_render_context(const Window *window) {
        RenderContext context;

        context.window = window;

        context.cube_buffer = create_vertexbuffer(&brl::builtin::CUBE_MESH);
        context.cone_buffer = create_vertexbuffer(&brl::builtin::CONE_MESH);
        context.cylinder_buffer = create_vertexbuffer(&brl::builtin::CYLINDER_MESH);
        context.sphere_buffer = create_vertexbuffer(&brl::builtin::SPHERE_MESH);
        context.plane_buffer = create_vertexbuffer(&brl::builtin::PLANE_MESH);

        context.object_shader = create_shader(
            brl::builtin::objectVertexShaderSource,
            brl::builtin::objectFragShaderSource
        );

        context.unlit_shader = create_shader(
            brl::builtin::objectVertexShaderSource,
            brl::builtin::unlitFragShaderSource
        );

        context.grid_shader = create_shader(
            brl::builtin::objectVertexShaderSource, 
            brl::builtin::gridFragShaderSource
        );

        context.instance_shader = create_shader(
            brl::builtin::instancedObjectVertexShaderSource,
            brl::builtin::instancedObjectFragShaderSource
        );

        global_render_context = context;

        return context;
    }

    void destroy_render_context(RenderContext* context) {

    }

    void use_shader(const Shader &shader) {
        glUseProgram(shader.program_id);
    }

    /* ---------------------------------- Cube ---------------------------------- */

    void draw_cube(const smath::matrix4x4 &transform, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;

        set_shader_uniform_matrix4(context.object_shader, transform, "model");
        set_shader_uniform_float4(context.object_shader, color, "color");
        draw_vertex_buffer(context.cube_buffer);
    }

    void draw_cube_instances(const InstanceData* data, const uint32_t count) {
        const RenderContext& context = global_render_context;

        brl::use_shader(context.instance_shader);
        brl::draw_vertexbuffer_instanced(context.cube_buffer, data, count);
        brl::use_shader(context.object_shader);
    }

    /* ---------------------------------- Cone ---------------------------------- */

    void draw_cone(const smath::matrix4x4 &transform, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;

        set_shader_uniform_matrix4(context.object_shader, transform, "model");
        set_shader_uniform_float4(context.object_shader, color, "color");
        draw_vertex_buffer(context.cone_buffer);
    }

    void draw_cone_instances(const InstanceData *data, uint32_t count) {
        const RenderContext& context = global_render_context;

        brl::use_shader(context.instance_shader);
        brl::draw_vertexbuffer_instanced(context.cone_buffer, data, count);
        brl::use_shader(context.object_shader);
    }

    /* -------------------------------- Cylinder -------------------------------- */

    void draw_cylinder(const smath::matrix4x4 &transform, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;

        set_shader_uniform_matrix4(context.object_shader, transform, "model");
        set_shader_uniform_float4(context.object_shader, color, "color");
        draw_vertex_buffer(context.cylinder_buffer);
    }

    void draw_cylinder_instances(const InstanceData *data, uint32_t count) {
        const RenderContext& context = global_render_context;

        brl::use_shader(context.instance_shader);
        brl::draw_vertexbuffer_instanced(context.cylinder_buffer, data, count);
        brl::use_shader(context.object_shader);
    }

    /* --------------------------------- Sphere --------------------------------- */


    void draw_sphere(const smath::matrix4x4 &transform, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;

        set_shader_uniform_matrix4(context.object_shader, transform, "model");
        set_shader_uniform_float4(context.object_shader, color, "color");
        draw_vertex_buffer(context.sphere_buffer);
    }

    void draw_sphere_instances(const InstanceData *data, uint32_t count) {
        const RenderContext& context = global_render_context;

        brl::use_shader(context.instance_shader);
        brl::draw_vertexbuffer_instanced(context.sphere_buffer, data, count); 
        brl::use_shader(context.object_shader);
    }

    void draw_sphere(const smath::vector3 position, const float radius, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;

        smath::vector4 i = {radius, 0.0f, 0.0f, 0.0f};
        smath::vector4 j = {0.0f, radius, 0.0f, 0.0f};
        smath::vector4 k = {0.0f, 0.0f, radius, 0.0f};
        smath::vector4 l = {position.x, position.y, position.z, 1.0f};
        smath::matrix4x4 transform = {i, j, k, l};
        set_shader_uniform_matrix4(context.object_shader, transform, "model");
        set_shader_uniform_float4(context.object_shader, color, "color");
        draw_vertex_buffer(context.sphere_buffer);
    }

    /* --------------------------------- Circle --------------------------------- */

    // Code adapted from raylib
    void draw_arc(const smath::matrix4x4 &transform, const float radius, float start_angle, float end_angle, uint segments, const Color &color) {
        const RenderContext& context = global_render_context;
        const float DEG2RAD = M_PI/180.0f;

        glDisable(GL_CULL_FACE);
        use_shader(context.unlit_shader);
        set_shader_uniform_matrix4(context.unlit_shader, transform, "model");
        set_shader_uniform_float4(context.unlit_shader, color.vector, "color");

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

    void draw_circle(const smath::matrix4x4 &transform, const float radius, const Color &color) {
        draw_arc(transform, radius, 0, 360, 180, color);
    }

    void draw_circle_lines(const smath::matrix4x4 &transform, const float radius, float line_width, const Color &color) {
        const RenderContext& context = global_render_context;

        using namespace smath;

        matrix4x4 transformation_matrix = transform;

        use_shader(context.unlit_shader);

        glLineWidth((GLfloat) line_width);
        set_shader_uniform_float4(context.unlit_shader, color.vector, "color");
        set_shader_uniform_matrix4(context.unlit_shader, transformation_matrix, "model");

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


    void draw_circle_lines(const smath::vector3 &position, const smath::vector3 &direction, const float radius, float line_width, const Color &color) {

        using namespace smath;

        quaternion rotation = normalize(quaternion_from_matrix3x3(matrix3x3_from_khat(normalize(direction))));

        transform transform_struct = {
            .translation = position,
            .rotation = rotation,
            .scale = {1, 1, 1}
        };
        matrix4x4 transformation_matrix = smath::matrix4x4_from_transform(transform_struct);
        
        use_shader(global_render_context.unlit_shader);

        glLineWidth((GLfloat) line_width);
        set_shader_uniform_float4(global_render_context.unlit_shader, color.vector, "color");
        set_shader_uniform_matrix4(global_render_context.unlit_shader, transformation_matrix, "model");

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

    void draw_plane(const smath::matrix4x4 &transform, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;

        set_shader_uniform_matrix4(context.object_shader, transform, "model");
        set_shader_uniform_float4(context.object_shader, color, "color");
        draw_vertex_buffer(context.sphere_buffer);
    }

    void draw_plane_instances(const InstanceData *data, uint32_t count) {
        const RenderContext& context = global_render_context;

        brl::use_shader(context.instance_shader);
        brl::draw_vertexbuffer_instanced(context.plane_buffer, data, count); 
        brl::use_shader(context.object_shader);
    }

    /* --------------------------------- Vector --------------------------------- */

    void draw_vector(const smath::vector3 &position, const smath::vector3 &vector, const float radius, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;

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
        
        draw_cylinder(matrix4x4_from_transform(cylinderTransform)*translation, color);
        draw_cone(matrix4x4_from_transform(coneTransform)*translation, color);
    }

    /* ---------------------------------- Line ---------------------------------- */

    void draw_line(const smath::vector3 &position1, const smath::vector3 &position2, const float radius, const smath::vector4 &color) {
        const RenderContext& context = global_render_context;
        
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

        draw_cylinder(matrix4x4_from_transform(cylinderTransform)*translation, color);
    }

    /* ---------------------------------- Mesh ---------------------------------- */

    void draw_mesh(const Mesh &mesh, const smath::matrix4x4 &transform, const Color &color) {
        const RenderContext& context = global_render_context;

        use_shader(context.object_shader);
        set_shader_uniform_matrix4(context.object_shader, transform, "model");
        set_shader_uniform_float4(context.object_shader, color.vector, "color");
        draw_vertex_buffer(mesh.buffer);
    }

    void draw_mesh_instances(const Mesh &mesh, const InstanceData *data, const uint32_t count) {
        const RenderContext& context = global_render_context;

        use_shader(context.instance_shader);
        draw_vertexbuffer_instanced(mesh.buffer, data, count);
        use_shader(context.object_shader);

    }

    void draw_model(const Model &model) {
        const RenderContext& context = global_render_context;

        set_shader_uniform_matrix4(context.object_shader, model.transform, "model");
        set_shader_uniform_float4(context.object_shader, model.color, "color");
        draw_vertex_buffer(model.mesh.buffer);
    }
    
}