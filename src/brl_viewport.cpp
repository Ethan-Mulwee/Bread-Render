#include "brl_viewport.hpp"
#include "brl_framebuffer.hpp"
#include "brl_render.hpp"
#include "imgui.h"

namespace brl {
    ViewportContext create_viewport_context(const char* name, const uint32_t MSAA) {
        ViewportContext viewportContext;

        viewportContext.framebuffer = create_framebuffer(1080, 1080, MSAA);
        if (MSAA) viewportContext.output_framebuffer = create_framebuffer(1080, 1080, false);
        viewportContext.MSAA = MSAA;

        viewportContext.hovered = false;
        viewportContext.focused = false;
        viewportContext.name = name;

        return viewportContext;
    }

    void begin_viewport(ViewportContext &viewport, Camera &camera) {

        camera.aspect = viewport.size.x / viewport.size.y;

        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Set instance uniforms
        use_shader(global_render_context.instance_shader);
        set_shader_uniform_matrix4(global_render_context.instance_shader, calculate_camera_view(camera), "view");
        set_shader_uniform_matrix4(global_render_context.instance_shader, calculate_camera_projection(camera), "projection");

        use_shader(global_render_context.unlit_shader);
        set_shader_uniform_matrix4(global_render_context.unlit_shader, calculate_camera_view(camera), "view");
        set_shader_uniform_matrix4(global_render_context.unlit_shader, calculate_camera_projection(camera), "projection");

        use_shader(global_render_context.object_shader);
        set_shader_uniform_matrix4(global_render_context.object_shader, calculate_camera_view(camera), "view");
        set_shader_uniform_matrix4(global_render_context.object_shader, calculate_camera_projection(camera), "projection");

        ImGui::Begin(viewport.name);

        viewport.size = ImGui::GetContentRegionAvail();
        viewport.position = ImGui::GetCursorPos();
        viewport.screen_position = ImGui::GetCursorScreenPos();
        viewport.hovered = ImGui::IsWindowHovered();
        viewport.focused = ImGui::IsWindowFocused();

        uint64_t textureID;
        brl::resize_framebuffer(&viewport.framebuffer, viewport.size.x, viewport.size.y);
        if (viewport.MSAA) {
            brl::resize_framebuffer(&viewport.output_framebuffer, viewport.size.x, viewport.size.y);
            textureID = viewport.output_framebuffer.texId;
        } else {
            textureID = viewport.framebuffer.texId;
        }

        ImGui::Image((ImTextureRef)(textureID), viewport.size, ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::SetCursorPos(viewport.position);

        bind_framebuffer(viewport.framebuffer);

        clear_render(viewport.background_color);

    }


    void end_viewport(ViewportContext &viewport, Camera &camera) {
        
        render_mode_solid();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);

        use_shader(global_render_context.grid_shader);
        set_shader_uniform_matrix4(global_render_context.grid_shader, calculate_camera_view(camera), "view");
        set_shader_uniform_matrix4(global_render_context.grid_shader, calculate_camera_projection(camera), "projection");

        smath::vector3 cameraPosition = calculate_camera_position(camera);
        const float size = 250.0f;
        smath::matrix4x4 gridTransform = {
            size, 0.0f, 0.0f,                           0.0f,
            0.0f, size, 0.0f,                           0.0f,
            0.0f, 0.0f, size,                           0.0f,
            -cameraPosition.x, 0.0f, -cameraPosition.z, 1.0f
        };
        set_shader_uniform_matrix4(global_render_context.grid_shader, gridTransform, "model");
        draw_vertex_buffer(global_render_context.plane_buffer);

        if (viewport.MSAA)
            brl::blit_framebuffer(viewport.framebuffer.fBO, viewport.output_framebuffer.fBO, viewport.framebuffer.width, viewport.framebuffer.height);
        unbind_framebuffer();



        ImGui::End();

    }

    void set_viewport_background(ViewportContext* viewportContext, const Color &color) {
        viewportContext->background_color = color;
    }
}