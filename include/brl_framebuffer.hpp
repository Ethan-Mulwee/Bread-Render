#ifndef BRL_FRAMEBUFFER
#define BRL_FRAMEBUFFER

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <cstdint>

namespace brl {
    struct Framebuffer {
        uint32_t fBO = 0;
        uint32_t texId = 0;
        uint32_t depthId = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t MSAA = 0;
    };

    void bind_framebuffer(const Framebuffer &buffer);

    void unbind_framebuffer();

    Framebuffer create_framebuffer(int32_t width, int32_t height, const uint32_t MSAA);

    void resize_framebuffer(Framebuffer* buffer, const int32_t width, const int32_t height);

    void blit_framebuffer(const uint32_t source, const uint32_t destination, const uint32_t width, const uint32_t height);
}

#endif