#include "brl.hpp"

#include <cstdlib>
#include <iostream>

#include "brl_builtin_shaders.hpp"

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::setVsync(false);
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport1 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport1");
    brl::ViewportContext viewport2 = brl::createViewportContext(&renderContext, 1920, 1080, "viewport2");
    brl::Camera camera1 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Camera camera2 = brl::createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    int cubeAmount = 500000;
    smath::matrix4x4* cubeTransforms = new smath::matrix4x4[cubeAmount];
    brl::InstanceData* cubeInstanceData = new brl::InstanceData[cubeAmount];
    for (int i = 0; i < cubeAmount; i++) {
        int positionIntX = rand() % 100000;
        int positionIntY = rand() % 100000;
        int positionIntZ = rand() % 100000;

        float positionX = positionIntX * 0.0001f;
        float positionY = positionIntY * 0.0001f;
        float positionZ = positionIntZ * 0.0001f;

        int colorIntR = rand() % 255;
        int colorIntG = rand() % 255;
        int colorIntB = rand() % 255;

        float colorR = colorIntR * (1.0f/255.0f);
        float colorG = colorIntG * (1.0f/255.0f);
        float colorB = colorIntB * (1.0f/255.0f);

        cubeTransforms[i] = smath::matrix4x4_from_translation({positionX, positionY, positionZ}) * smath::matrix4x4_from_scale(0.1f);
        cubeInstanceData[i].transform = smath::transpose(cubeTransforms[i]);
        cubeInstanceData[i].color = smath::vector4{colorR, colorG, colorB, 1.0f};
        // cubeTransposedTransforms[i] = smath::matrix4x4_from_identity();
    }


    brl::MeshData utahTeapotMeshData = brl::parseObj("../demo/OBJs/Utah-Teapot.obj");
    brl::Mesh utahTeapotMesh = brl::createMesh(&utahTeapotMeshData);
    brl::MeshData cubeMeshData = brl::parseObj("../demo/OBJs/Primitive-Cube.obj");
    brl::InstancedVertexBuffer instancedVertexBuffer = brl::createInstancedVertexBuffer(&cubeMeshData);
    brl::Shader instanceShader = brl::createShader(brl::builtin::instancedObjectVertexShaderSource, brl::builtin::instancedObjectFragShaderSource);



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
                // brl::drawCube(renderContext, smath::matrix4x4_from_identity(), smath::vector4{1.0f, 1.0f, 1.0f, 1.0f});

                brl::useShader(instanceShader);
                setShaderUniformMatrix4(instanceShader, calculateCameraView(camera1), "view");
                setShaderUniformMatrix4(instanceShader, calculateCameraProjection(camera1), "projection");
                
                // int drawnAmount = rand() % cubeAmount;
                int drawnAmount = cubeAmount;

                brl::bindInstancedVertexBuffer(instancedVertexBuffer);
                brl::setInstancedVertexBufferData(instancedVertexBuffer, cubeInstanceData, drawnAmount);
                glDrawElementsInstanced(GL_TRIANGLES, instancedVertexBuffer.size, GL_UNSIGNED_INT, 0, drawnAmount);

                

                // for (int i = 0; i < cubeAmount; i++) {
                //     brl::drawCube(renderContext, cubeTransforms[i]);
                // }
            brl::endViewport(viewport1, camera1);

            // brl::beginViewport(viewport2, camera2);

            //     ImGui::Text("This is text dispalyed ontop of the viewport!");
            
            //     brl::drawMesh(renderContext, utahTeapotMesh, smath::matrix4x4_from_scale(0.4f), smath::vector4{1.0f, 0.1f, 0.0f, 1.0f});

            //     brl::renderModeWireframe();
            //     brl::drawSphere(renderContext, smath::vector3{2.2f, 0.0f, 0.0f}, 1.0f, smath::vector4{0.0f, 0.9f, 0.1f});
            //     brl::drawCylinder(renderContext, smath::matrix4x4_from_translation(smath::vector3{-2.2f, 0.0f, 0.0f}));

            //     brl::renderModeSolid();
            //     brl::drawVector(renderContext, {0.0f, 0.0f, 2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f);

            //     brl::renderModeTransparent();
            //     brl::drawVector(renderContext, {0.0f, 0.0f, -2.2f}, {1.0f, 1.0f, 1.0f}, 0.2f, {1.0f, 0.5f, 1.0f, 0.3f});

            // brl::endViewport(viewport2, camera2);
            
        brl::endRender();
    }

    brl::destroyWindow(window);
}