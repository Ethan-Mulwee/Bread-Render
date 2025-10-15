#include "brl_shader.hpp"

#include <iostream>

namespace brl {
    unsigned int getCompiledShader(unsigned int shaderType, const char* shaderSource) {
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

    Shader createShader(const char *vertShaderSource, const char *fragShaderSource) {
        Shader shader;

        shader.programId = glCreateProgram();

        unsigned int vertShader = getCompiledShader(GL_VERTEX_SHADER, vertShaderSource);
        unsigned int fragShader = getCompiledShader(GL_FRAGMENT_SHADER, fragShaderSource);

        glAttachShader(shader.programId, vertShader);
        glAttachShader(shader.programId, fragShader);

        glLinkProgram(shader.programId);

        // GLint linkResult;
        // glGetProgramiv(shader.programId, GL_LINK_STATUS, &linkResult);
        // if (linkResult == GL_FALSE) {
        //     std::cout << "shader linking failed \n";

        //     int length;
        //     glGetProgramiv(shader.programId, GL_INFO_LOG_LENGTH, &length);

        //     GLchar* strInfoLog = new GLchar[length + 1];
        //     glGetShaderInfoLog(shader.programId, length, &length, strInfoLog);

        //     fprintf(stderr, "Shader linking error: %s\n", strInfoLog);
        //     delete[] strInfoLog;
        // }

        glValidateProgram(shader.programId);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return shader;
    }

    void setShaderUniformMatrix4(const Shader &shader, const smath::matrix4x4 &mat4, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
        glUniformMatrix4fv(myLoc, 1, GL_TRUE, &mat4.data[0][0]);
    }

    void setShaderUniformInt(const Shader &shader, const int v, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
        glUniform1i(myLoc, v);
    }

    void setShaderUniformFloat3(const Shader &shader, const smath::vector3 &v, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
        glProgramUniform3fv(shader.programId, myLoc, 1, (float*)&v);
    }

    void setShaderUniformFloat4(const Shader &shader, const smath::vector4 &v, const std::string &name) {
        GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
        glProgramUniform4fv(shader.programId, myLoc, 1, (float*)&v);
    }
}