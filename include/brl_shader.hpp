#ifndef BRL_SHADER
#define BRL_SHADER

#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace brl {
    struct Shader {
        unsigned int programId;
    };

    Shader createShader(const std::string &vertShaderPath, const std::string &fragShaderPath);
}

#endif