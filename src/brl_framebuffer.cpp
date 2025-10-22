#include "brl_framebuffer.hpp"
#include "brl_window.hpp"

namespace brl {
    void bindFramebuffer(const Framebuffer &buffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer.fBO);
        glViewport(0,0, buffer.width, buffer.height);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void unbindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Framebuffer createFramebuffer(int32_t width, int32_t height, const bool multiSample) {
        Framebuffer buffer;

        buffer.width = width;
        buffer.height = height;

        unsigned short textureType;
        if (multiSample) {
            textureType = GL_TEXTURE_2D_MULTISAMPLE;
            buffer.multiSampled = true;
        }
        else {
            textureType = GL_TEXTURE_2D;
            buffer.multiSampled = false;
        }

        glGenFramebuffers(1, &buffer.fBO);
        glBindFramebuffer(GL_FRAMEBUFFER, buffer.fBO);
        glCreateTextures(textureType, 1, &buffer.texId);
        glBindTexture(textureType, buffer.texId);

        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexImage2DMultisample(textureType, 4, GL_RGB, buffer.width, buffer.height, GL_TRUE);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType, buffer.texId, 0);

        glCreateTextures(textureType, 1, &buffer.depthId);
        glBindTexture(textureType, buffer.depthId);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, buffer.width, buffer.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glTexImage2DMultisample(textureType, 0, GL_DEPTH24_STENCIL8, buffer.width, buffer.height, GL_TRUE);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, textureType, buffer.depthId, 0);

        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(buffer.texId, buffers);
        
        unbindFramebuffer();

        return buffer;
    }

    void resizeFramebuffer(Framebuffer *buffer, const int32_t width, const int32_t height) {
        buffer->width = width;
        buffer->height = height;

        unsigned short textureType;
        if (buffer->multiSampled) {
            textureType = GL_TEXTURE_2D_MULTISAMPLE;
        }
        else {
            textureType = GL_TEXTURE_2D;
        }

        glBindTexture(textureType, buffer->texId);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer->width, buffer->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexImage2DMultisample(textureType, 4, GL_RGB, buffer->width, buffer->height, GL_TRUE);
        glBindTexture(textureType, buffer->depthId);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, buffer->width, buffer->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glTexImage2DMultisample(textureType, 0, GL_DEPTH24_STENCIL8, buffer->width, buffer->height, GL_TRUE);
        glBindTexture(textureType, 0);
    }

    void blitFramebuffer(const uint32_t source, const uint32_t destination, const uint32_t width, const uint32_t height) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, source);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
}