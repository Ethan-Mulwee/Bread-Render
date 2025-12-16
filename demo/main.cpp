#include "brl.hpp"

#include <cstdlib>

int main() {
    brl::Window* window = brl::create_window(1920, 1080, "test");
    brl::set_vsync(false);
    brl::RenderContext renderContext = brl::create_render_context(window);
    brl::ViewportContext viewport1 = brl::create_viewport_context(&renderContext, "viewport1", 4);
    brl::ViewportContext viewport2 = brl::create_viewport_context(&renderContext, "viewport2", 4);
    brl::Camera camera1 = brl::create_camera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::create_camera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    brl::MeshData utahTeapotMeshData = brl::parse_obj("../demo/OBJs/Utah-Teapot.obj");
    brl::Mesh utahTeapotMesh = brl::create_mesh(&utahTeapotMeshData);
    brl::MeshData cubeMeshData = brl::parse_obj("../demo/OBJs/Primitive-Cube.obj");

    while (!brl::window_should_close(window)) {
        brl::update_window(window);
        if (viewport1.hovered) brl::update_camera(&camera1, window);
        if (viewport2.hovered) brl::update_camera(&camera2, window);

        brl::begin_render(window);

            ImGui::Begin("Stats");
            ImGui::Text("Delta time: %fms", window->deltaTime*1000.0);
            ImGui::Text("Hello World");
            ImGui::End();

            brl::begin_viewport(viewport1, camera1);
                brl::draw_cube(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
            brl::end_viewport(viewport1, camera1);

            brl::begin_viewport(viewport2, camera2);

                ImGui::Text("This is text dispalyed ontop of the viewport!");
                ImGui::Text("size: %f, %f", viewport2.size.x, viewport2.size.y);

                brl::Color arc_color = brl::FREYA_BLUE;
                arc_color.vector.w = 0.8f;
                
                brl::render_mode_transparent();
                
                brl::draw_circle_lines(renderContext, smath::vector3{0.0, 2.0, 3.0}, smath::vector3{0.0, 1.0, 0.0}, 1.0f, 3.0f, brl::FREYA_RED);
                brl::draw_circle_lines(renderContext, smath::vector3{0.0, 2.0, 3.0}, smath::vector3{0.0, 0.0, 1.0}, 1.0f, 3.0f, brl::FREYA_BLUE);
            
                brl::draw_mesh(renderContext, utahTeapotMesh, smath::matrix4x4_from_scale(0.4f), brl::ORANGE);

                brl::render_mode_wireframe();
                brl::draw_sphere(renderContext, smath::vector3{2.2f, 0.0f, 0.0f}, 1.0f, smath::vector4{0.0f, 0.9f, 0.1f});
                brl::draw_cylinder(renderContext, smath::matrix4x4_from_translation(smath::vector3{-2.2f, 0.0f, 0.0f}));
                
                brl::render_mode_solid();
                brl::draw_vector(renderContext, {0.0f, 0.0f, 2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f);
                
                brl::render_mode_transparent();
                brl::draw_vector(renderContext, {0.0f, 0.0f, -2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});
                // rendering order is an issue for transparent drawing, this needs to be fixed later with the render grouping system
                brl::draw_arc(renderContext, smath::matrix4x4_from_identity(), 2.0f, 0.0f, 120.0f, 180, arc_color);
                brl::draw_line(renderContext, {0.0f, 0.0f, -2.7f}, {0.0f, 1.0f, -3.9f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});
                

            brl::end_viewport(viewport2, camera2);
            
        brl::end_render();
    }

    brl::destroy_render_context(&renderContext);
    brl::destroy_window(window);
}