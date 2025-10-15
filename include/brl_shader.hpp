#ifndef BRL_SHADER
#define BRL_SHADER

#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "smath.hpp"

namespace brl {
    struct Shader {
        unsigned int programId;
    };

    Shader createShaderFromFile(const std::string &vertShaderPath, const std::string &fragShaderPath);

    Shader createShader(const char* vertShaderSource, const char* fragShaderSource);

    void useShader(const Shader &shader);

    void setShaderUniformMatrix4(const Shader &shader, const smath::matrix4x4 &mat4, const std::string &name);

    void setShaderUniformInt(const Shader &shader, const int v, const std::string &name);

    void setShaderUniformFloat3(const Shader &shader, const smath::vector3 &v, const std::string &name);

    void setShaderUniformFloat4(const Shader &shader, const smath::vector4 &v, const std::string &name);
}

#endif