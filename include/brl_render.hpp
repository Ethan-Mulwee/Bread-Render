#ifndef BRL_RENDER
#define BRL_RENDER

#include "brl_window.hpp"

namespace brl {
    void beginRender(Window* window);
    void clearRender(smath::vector4 color = {0.2f, 0.2f, 0.2f, 0.2f});
    void endRender();
}

#endif