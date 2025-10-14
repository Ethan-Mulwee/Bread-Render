#ifndef BRL_SHADER
#define BRL_SHADER

#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace brl {
    struct Shader {
        unsigned int programId;
    };

    Shader createShaderFromFile(const std::string &vertShaderPath, const std::string &fragShaderPath);

    Shader createShader(const char* vertShaderSource, const char* fragShaderSource);
}

#endif