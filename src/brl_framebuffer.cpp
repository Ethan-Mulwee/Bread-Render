#include "brl_framebuffer.hpp"

namespace brl {
    void bindFramebuffer(const Framebuffer *buffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer->fBO);
        glViewport(0,0, buffer->width, buffer->height);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void unbindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Framebuffer createFramebuffer(int32_t width, int32_t height) {
        Framebuffer buffer;

        buffer.width = width;
        buffer.height = height;

        glGenFramebuffers(1, &buffer.fBO);
        glBindFramebuffer(GL_FRAMEBUFFER, buffer.fBO);
        glCreateTextures(GL_TEXTURE_2D, 1, &buffer.texId);
        glBindTexture(GL_TEXTURE_2D, buffer.texId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texId, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &buffer.depthId);
        glBindTexture(GL_TEXTURE_2D, buffer.depthId);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, buffer.width, buffer.height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, buffer.depthId, 0);

        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(buffer.texId, buffers);
        
        unbindFramebuffer();

        return buffer;
    }
}