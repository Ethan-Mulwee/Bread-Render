#include "brl.hpp"

#include <cstdlib>
#include <renderdoc_app.h>
#include <dlfcn.h>

#include <iostream>

#include <random>

int main() {
    brl::Window* window = brl::create_window(1920, 1080, "test");
    brl::RenderContext render_context = brl::create_render_context(window);
    brl::ViewportContext viewport1 = brl::create_viewport_context("viewport1", 4);
    brl::ViewportContext viewport2 = brl::create_viewport_context("viewport2", 4);
    brl::Camera camera1 = brl::create_camera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::create_camera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    brl::MeshData utahTeapotMeshData = brl::parse_obj("../demo/OBJs/Utah-Teapot.obj");
    brl::Mesh utahTeapotMesh = brl::create_mesh(&utahTeapotMeshData);
    brl::MeshData cubeMeshData = brl::parse_obj("../demo/OBJs/Primitive-Cube.obj");

    #if BRL_DEBUG
        RENDERDOC_API_1_1_2 *rdoc_api = NULL;

        // At init, on linux/android.
        // For android replace librenderdoc.so with libVkLayer_GLES_RenderDoc.so
        if(void *mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD))
        {
            pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
            int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
            assert(ret == 1);
        }
    #endif

    brl::DynamicVertexbuffer test = brl::create_dynamic_vertexbuffer(&utahTeapotMeshData);
    brl::GLSyncBuffer test_sync_buffer;
    test_sync_buffer.init(test, 2);
    // test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::WHITE);
    test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::PURE_GREEN);
    test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::WHITE);
    test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::WHITE);
    test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::WHITE);
    test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::WHITE);
    // test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::WHITE);
    
    
    
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
                brl::draw_cube(smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
            brl::end_viewport(viewport1, camera1);

            brl::begin_viewport(viewport2, camera2);

                ImGui::Text("This is text dispalyed ontop of the viewport!");
                ImGui::Text("size: %f, %f", viewport2.size.x, viewport2.size.y);

                // brl::Color arc_color = brl::FREYA_BLUE;
                // arc_color.vector.w = 0.8f;
                
                // brl::render_mode_transparent();
                
                // brl::draw_circle_lines(smath::vector3{0.0, 2.0, 3.0}, smath::vector3{0.0, 1.0, 0.0}, 1.0f, 3.0f, brl::FREYA_RED);
                // brl::draw_circle_lines(smath::vector3{0.0, 2.0, 3.0}, smath::vector3{0.0, 0.0, 1.0}, 1.0f, 3.0f, brl::FREYA_BLUE);
            
                // brl::draw_mesh(utahTeapotMesh, smath::matrix4x4_from_scale(0.4f), brl::ORANGE);

                // brl::render_mode_wireframe();
                // brl::draw_sphere(smath::vector3{2.2f, 0.0f, 0.0f}, 1.0f, smath::vector4{0.0f, 0.9f, 0.1f});
                // brl::draw_cylinder(smath::matrix4x4_from_translation(smath::vector3{-2.2f, 0.0f, 0.0f}));
                
                // brl::render_mode_solid();
                // brl::draw_vector({0.0f, 0.0f, 2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f);
                
                // brl::render_mode_transparent();
                // brl::draw_vector({0.0f, 0.0f, -2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});
                // // rendering order is an issue for transparent drawing, this needs to be fixed later with the render grouping system
                // brl::draw_arc(smath::matrix4x4_from_identity(), 2.0f, 0.0f, 120.0f, 180, arc_color);
                // brl::draw_line({0.0f, 0.0f, -2.7f}, {0.0f, 1.0f, -3.9f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});
                // test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 1 * 0.1f, rand() % 1 * 0.1f, rand() % 1 * 0.1f}), brl::WHITE);
                test_sync_buffer.clear();
                test_sync_buffer.add(smath::matrix4x4_from_translation(smath::vector3{rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f}), brl::PURE_GREEN);

                brl::use_shader(render_context.instance_shader);
                test_sync_buffer.shrink_to_fit();
                test_sync_buffer.sync();
                brl::draw_vertexbuffer_instanced2(test_sync_buffer.vertexbuffer, test_sync_buffer.used);
                brl::use_shader(render_context.object_shader);

                std::cout << test_sync_buffer.size << "\n";
                // brl::draw_sphere_instances(test_sync_buffer.databuffer, test_sync_buffer.used);


                // for (int i = 0; i < 100000; i++)
                //     brl::draw_sphere(smath::vector3{0.0f, 0.0f, 0.0f}, 1.0f);
            brl::end_viewport(viewport2, camera2);
            
        brl::end_render();
    }

    brl::destroy_render_context(&render_context);
    brl::destroy_window(window);
}