#include "brl_shader.hpp"

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
}