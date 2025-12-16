#include "brl_shader.hpp"

#include <iostream>

namespace brl {
    unsigned int get_compiled_shader(unsigned int shaderType, const char* shaderSource) {
        unsigned int shader_id = glCreateShader(shaderType);

        glShaderSource(shader_id, 1, &shaderSource, nullptr);
        glCompileShader(shader_id);

        GLint result;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

            GLchar* strInfoLog = new GLchar[length + 1];
            glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

            fprintf(stderr, "Compile error in shader: %s\n", strInfoLog);
            delete[] strInfoLog;
        }

        return shader_id;
    }

    Shader create_shader(const char *vertShaderSource, const char *fragShaderSource) {
        Shader shader;

        shader.program_id = glCreateProgram();

        unsigned int vertShader = get_compiled_shader(GL_VERTEX_SHADER, vertShaderSource);
        unsigned int fragShader = get_compiled_shader(GL_FRAGMENT_SHADER, fragShaderSource);

        glAttachShader(shader.program_id, vertShader);
        glAttachShader(shader.program_id, fragShader);

        glLinkProgram(shader.program_id);

        glValidateProgram(shader.program_id);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return shader;
    }

    void set_shader_uniform_matrix4(const Shader &shader, const smath::matrix4x4 &mat4, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.program_id, name.c_str());
        glUniformMatrix4fv(myLoc, 1, GL_FALSE, &mat4.data[0][0]);
    }

    void set_shader_uniform_int(const Shader &shader, const int v, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.program_id, name.c_str());
        glUniform1i(myLoc, v);
    }

    void set_shader_uniform_float3(const Shader &shader, const smath::vector3 &v, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.program_id, name.c_str());
        glProgramUniform3fv(shader.program_id, myLoc, 1, (float*)&v);
    }

    void set_shader_uniform_float4(const Shader &shader, const smath::vector4 &v, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.program_id, name.c_str());
        glProgramUniform4fv(shader.program_id, myLoc, 1, (float*)&v);
    }
}