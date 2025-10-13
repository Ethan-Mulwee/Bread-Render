#include "brl_vertexbuffer.hpp"

namespace brl {
    void bindVertexbuffer(const Vertexbuffer &buffer) {
        glBindVertexArray(buffer.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    }

    void unbindVertexBuffer() {
        glBindVertexArray(0);
    }
}