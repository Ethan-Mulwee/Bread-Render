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
    };

    void bindFramebuffer(const Framebuffer &buffer);

    void unbindFramebuffer();

    Framebuffer createFramebuffer(int32_t width, int32_t height);
}

#endif