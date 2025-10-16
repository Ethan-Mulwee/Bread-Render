# Bread Render Layer (BRL)

Bread Render Layer is a layer overtop of glfw, glew, and Dear ImGui along with Simple-Math desgined to make creating 3D applications with OpenGL easier. Many of the design decision are inspired by Raylib with simplicity in mind. And are written using minimal C++ features mostly C style.

Basic demo
```cpp
#include "brl.hpp"

int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport = brl::createViewportContext(&renderContext, 1920, 1080, "viewport");
    brl::Camera camera = brl::createCamera({0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    while (!brl::windowShouldClose(window)) {
        brl::updateWindow(window);
        if (viewport.hovered) brl::updateCamera(&camera, window);

        brl::beginRender(window);
            brl::clearRender();

            // ImGui functions can be called here
            ImGui::Begin("Hello World");
            ImGui::Text("Hello World!");
            ImGui::End();

            brl::beginViewport(viewport, camera);
                // Any ImGui code executed between begin and end viewport appears in the viewport
                ImGui::Text("This is text dispalyed ontop of the viewport!");
                // Calling draw functions between begin and end viewport draws to that viewport
                brl::drawCube(renderContext, smath::matrix4x4_from_identity());
            brl::endViewport(viewport, camera);
        brl::endRender();
    }
}
```

Vsync is enabled by default it can be disabled with `brl::setVsync(false);` after creating a window.

# Cloning & Building

The following will create the static library under `build/libbrl.a` and a demo executable `build/brl-demo`
```bash
    git clone --recursive https://github.com/Ethan-Mulwee/Bread-Render.git && cd Bread-Render/
    cmake -S . -B build/
    cmake --build build/
```

# Using as a Submodule with CMake
To skip building the demo executable set `BRL_ONLY_LIB` to true (note this doesn't actually work at the moment I'll fix that later)

Adding as a submodule
```bash
git submodule add https://github.com/Ethan-Mulwee/Bread-Render.git ext/brl
cd ext/brl/
git submodule init && git submodule update
```

Example `CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(brl-test)

add_subdirectory(ext/brl)
set(BRL_LIB_ONLY TRUE)

add_executable(brl-test main.cpp)

target_include_directories(brl-test PUBLIC ext/brl/include)
target_link_directories(brl-test PRIVATE ext/brl)
target_link_libraries(brl-test brl)
```
Then build the project how you would any other cmake project
```bash
cmake -S . -B build/
cmake --build build/
```