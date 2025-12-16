#ifndef BRL_SHADER
#define BRL_SHADER

#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "smath.hpp"

namespace brl {
    struct Shader {
        unsigned int program_id;
    };

    Shader create_shader_from_file(const std::string &vertShaderPath, const std::string &fragShaderPath);

    Shader create_shader(const char* vertShaderSource, const char* fragShaderSource);

    void use_shader(const Shader &shader);

    void set_shader_uniform_matrix4(const Shader &shader, const smath::matrix4x4 &mat4, const std::string &name);

    void set_shader_uniform_int(const Shader &shader, const int v, const std::string &name);

    void set_shader_uniform_float3(const Shader &shader, const smath::vector3 &v, const std::string &name);

    void set_shader_uniform_float4(const Shader &shader, const smath::vector4 &v, const std::string &name);
}

#endif