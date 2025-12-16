#include "brl.hpp"

#include <cstdlib>

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::setVsync(false);
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport1 = brl::createViewportContext(&renderContext, "viewport1", 4);
    brl::ViewportContext viewport2 = brl::createViewportContext(&renderContext, "viewport2", 4);
    brl::Camera camera1 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    brl::MeshData utahTeapotMeshData = brl::parseObj("../demo/OBJs/Utah-Teapot.obj");
    brl::Mesh utahTeapotMesh = brl::createMesh(&utahTeapotMeshData);
    brl::MeshData cubeMeshData = brl::parseObj("../demo/OBJs/Primitive-Cube.obj");

    while (!brl::windowShouldClose(window)) {
        brl::updateWindow(window);
        if (viewport1.hovered) brl::updateCamera(&camera1, window);
        if (viewport2.hovered) brl::updateCamera(&camera2, window);

        brl::beginRender(window);
            brl::clearRender();

            ImGui::Begin("Stats");
            ImGui::Text("Delta time: %fms", window->deltaTime*1000.0);
            ImGui::Text("Hello World");
            ImGui::End();

            brl::beginViewport(viewport1, camera1);
                brl::drawCube(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});
            brl::endViewport(viewport1, camera1);

            brl::beginViewport(viewport2, camera2);

                ImGui::Text("This is text dispalyed ontop of the viewport!");
                ImGui::Text("size: %f, %f", viewport2.size.x, viewport2.size.y);

                brl::draw_arc(renderContext, smath::matrix4x4_from_identity(), 2.0f, 0.0f, 120.0f, 180, brl::FREYA_BLUE);

                brl::draw_circle_lines(renderContext, smath::vector3{0.0, 2.0, 3.0}, smath::vector3{0.0, 1.0, 0.0}, 1.0f, 3.0f, brl::FREYA_RED);
                brl::draw_circle_lines(renderContext, smath::vector3{0.0, 2.0, 3.0}, smath::vector3{0.0, 0.0, 1.0}, 1.0f, 3.0f, brl::FREYA_BLUE);
            
                brl::drawMesh(renderContext, utahTeapotMesh, smath::matrix4x4_from_scale(0.4f), brl::ORANGE);

                brl::renderModeWireframe();
                brl::drawSphere(renderContext, smath::vector3{2.2f, 0.0f, 0.0f}, 1.0f, smath::vector4{0.0f, 0.9f, 0.1f});
                brl::drawCylinder(renderContext, smath::matrix4x4_from_translation(smath::vector3{-2.2f, 0.0f, 0.0f}));

                brl::renderModeSolid();
                brl::drawVector(renderContext, {0.0f, 0.0f, 2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f);

                brl::renderModeTransparent();
                brl::drawVector(renderContext, {0.0f, 0.0f, -2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});
                brl::drawLine(renderContext, {0.0f, 0.0f, -2.7f}, {0.0f, 1.0f, -3.9f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});
                

            brl::endViewport(viewport2, camera2);
            
        brl::endRender();
    }

    brl::destroyRenderContext(&renderContext);
    brl::destroyWindow(window);
}