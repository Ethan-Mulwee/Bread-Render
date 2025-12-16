#ifndef BRL_RENDER
#define BRL_RENDER

#include "brl_window.hpp"
#include "brl_vertexbuffer.hpp"
#include "brl_shader.hpp"
#include "brl_color.hpp"


namespace brl {
    struct RenderContext {
        
        const Window* window;

        Shader object_shader;
        Shader unlit_shader;
        Shader grid_shader;
        Shader instance_shader;

        Vertexbuffer sphere_buffer;
        Vertexbuffer cylinder_buffer;
        Vertexbuffer cone_buffer;
        Vertexbuffer cube_buffer;
        Vertexbuffer plane_buffer;
        
    };

    RenderContext create_render_context(const Window* window);
    void destroy_render_context(RenderContext* context);

    void begin_render(Window* window);
    void clear_render(const Color &color = {0.2f, 0.2f, 0.2f, 0.2f});
    void end_render();

    void render_mode_solid();
    void render_mode_wireframe(float line_width = 1.0f);
    void render_mode_transparent();

    void draw_cube(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_cube_instances(const RenderContext &context, const InstanceData* data, const uint32_t count);

    void draw_cone(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_cone_instances(const RenderContext &context, const InstanceData* data, uint32_t count);

    void draw_cylinder(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_cylinder_instances(const RenderContext &context, const InstanceData* data, uint32_t count);

    void draw_sphere(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color  = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_sphere(const RenderContext &context, const smath::vector3 position, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    // void drawSphereWire(const RenderContext &context, const smath::vector3 position, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_sphere_instances(const RenderContext &context, const InstanceData* data, uint32_t count);

    // void drawPolygon();

    void draw_arc(const RenderContext &context, const smath::matrix4x4 &transform, const float radius, float start_angle, float end_angle, uint segments = 180, const Color &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_circle(const RenderContext &context, const smath::matrix4x4 &transform, const float radius, float line_width = 1.0f, const Color &color = {1.0f, 1.0f, 1.0f, 1.0f});

    void draw_circle_lines(const RenderContext &context, const smath::matrix4x4 &transform, const float radius, float line_width = 1.0f, const Color &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_circle_lines(const RenderContext &context, const smath::vector3 &position, const smath::vector3 &direction, const float radius, float line_width = 1.0f, const Color &color = {1.0f, 1.0f, 1.0f, 1.0f});

    void draw_plane(const RenderContext &context, const smath::matrix4x4 &transform, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
    void draw_plane_instances(const RenderContext &context, const InstanceData* data, uint32_t count);

    void draw_vector(const RenderContext &context, const smath::vector3 &position, const smath::vector3 &vector, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});

    void draw_line(const RenderContext &context, const smath::vector3 &position1, const smath::vector3 &position2, const float radius, const smath::vector4 &color = {1.0f, 1.0f, 1.0f, 1.0f});

    void draw_mesh(const RenderContext &context, const Mesh &mesh, const smath::matrix4x4 &transform, const Color &color);
    void draw_mesh_instances(const RenderContext &context, const Mesh &mesh, const InstanceData* data, const uint32_t amount);
    
    void draw_model(const RenderContext &context, const Model &model);


}

#endif