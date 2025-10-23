#ifndef BRL_BUILTIN_SHADERS
#define BRL_BUILTIN_SHADERS

namespace brl {
    namespace builtin {
        static const char* objectVertexShaderSource = 
        R"(#version 330 core
    
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;
    
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform vec4 color;
        uniform mat4 depthBiasMVP;
    
        out vec3 WorldPos;
        out vec3 Normal;
        out vec4 Color;
        out vec4 ShadowCoord;
    
        void main() {
            Color = color;
            WorldPos = vec3(model * vec4(aPosition, 1.0));
            Normal = aNormal;
        
            gl_Position =  projection * view * model * vec4(aPosition, 1.0f);
            ShadowCoord = depthBiasMVP * vec4(aPosition, 1.0f);
        })";

        static const char* objectFragShaderSource = 
        R"(#version 330 core
    
        in vec4 Color;
        in vec3 Normal;
        in vec3 WorldPos;
        in vec4 ShadowCoord;
        layout(location = 0) out vec4 color;
    
        uniform mat4 model;
        uniform sampler2D shadowMap;
    
        void main() {
            mat3 normalMatrix = transpose(inverse(mat3(model)));
            vec3 normal = normalize(normalMatrix * Normal);
    


            float cosTheta = clamp( dot( normal,normalize(vec3(1.0,2.0,-0.4)) ), 0,1 );
            float bias = 0.001*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
            bias = clamp(bias, 0,0.01);
            float visibility = 1.0;
            if ( texture( shadowMap, ShadowCoord.xy ).z  <  ShadowCoord.z-bias){
                visibility = 0.5;
            }

            float light = dot(normal, normalize(vec3(1.0,2.0,-0.4)));
            light = clamp(light, 0.0, 1.0);
            light += 0.1;
    
            float light2 = dot(normal, normalize(vec3(-1.3,-0.2,-1.0)));
            light2 = clamp(light2, 0.0, 1.0);
            light2 *= 0.2;
    
            light += light2;
    
    
            color = vec4(vec3(Color)*light*visibility, Color.a);
        })";

        static const char* instancedObjectVertexShaderSource = 
        R"(#version 330 core
    
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in mat4 aInstanceMatrix;
        layout(location = 6) in vec4 aColor;
    
        uniform mat4 view;
        uniform mat4 projection;
    
        out vec3 WorldPos;
        out vec3 Normal;
        out vec4 Color;
        out mat4 ModelTransform;
    
        void main() {
        Color = aColor;
        ModelTransform = aInstanceMatrix;
        WorldPos = vec3(aInstanceMatrix * vec4(aPosition, 1.0));
        Normal = aNormal;
    
        gl_Position =  projection * view * aInstanceMatrix * vec4(aPosition, 1.0f);
        })";

        static const char* instancedObjectFragShaderSource = 
        R"(#version 330 core
    
        in vec4 Color;
        in vec3 Normal;
        in vec3 WorldPos;
        in mat4 ModelTransform;
        layout(location = 0) out vec4 color;
    
        void main() {
            mat3 normalMatrix = transpose(inverse(mat3(ModelTransform)));
            vec3 normal = normalize(normalMatrix * Normal);
    
            float light = dot(normal, normalize(vec3(1.0,2.0,-0.4)));
            light = clamp(light, 0.0, 1.0);
            light += 0.1;
    
            float light2 = dot(normal, normalize(vec3(-1.3,-0.2,-1.0)));
            light2 = clamp(light2, 0.0, 1.0);
            light2 *= 0.2;
    
            light += light2;
    
    
            color = vec4(vec3(Color)*light, Color.a);
        })";
    
        static const char* gridFragShaderSource = 
        R"(#version 330 core
    
        in vec3 WorldPos;
        layout(location = 0) out vec4 color;
    
        float pristineGrid( vec2 uv, vec2 lineWidth)
        {
            vec2 ddx = dFdx(uv);
            vec2 ddy = dFdy(uv);
            vec2 uvDeriv = vec2(length(vec2(ddx.x, ddy.x)), length(vec2(ddx.y, ddy.y)));
            bvec2 invertLine = bvec2(lineWidth.x > 0.5, lineWidth.y > 0.5);
            vec2 targetWidth = vec2(
            invertLine.x ? 1.0 - lineWidth.x : lineWidth.x,
            invertLine.y ? 1.0 - lineWidth.y : lineWidth.y
            );
            vec2 drawWidth = clamp(targetWidth, uvDeriv, vec2(0.5));
            vec2 lineAA = uvDeriv * 1.5;
            vec2 gridUV = abs(fract(uv) * 2.0 - 1.0);
            gridUV.x = invertLine.x ? gridUV.x : 1.0 - gridUV.x;
            gridUV.y = invertLine.y ? gridUV.y : 1.0 - gridUV.y;
            vec2 grid2 = smoothstep(drawWidth + lineAA, drawWidth - lineAA, gridUV);
    
            grid2 *= clamp(targetWidth / drawWidth, 0.0, 1.0);
            grid2 = mix(grid2, targetWidth, clamp(uvDeriv * 2.0 - 1.0, 0.0, 1.0));
            grid2.x = invertLine.x ? 1.0 - grid2.x : grid2.x;
            grid2.y = invertLine.y ? 1.0 - grid2.y : grid2.y;
            return mix(grid2.x, 1.0, grid2.y);
        }
    
        void main() {
            float gridAlpha = pristineGrid(WorldPos.xz, vec2(0.01,0.01));
            vec3 gridColor = vec3(0.35,0.35,0.35);
            if (WorldPos.x < 0.02 && WorldPos.x > -0.02) {
            gridColor = vec3(1.000,0.200,0.322);
            } else if (WorldPos.z < 0.02 && WorldPos.z > -0.02) {
            gridColor = vec3(0.157,0.565,1.0);
            // gridColor = vec3(0.471,0.863,0.000); Green
            }
    
            color = vec4(gridColor, gridAlpha);
        })";

        static const char* shadowVertShader =
        R"(#version 330 core

        // Input vertex data, different for all executions of this shader.
        layout(location = 0) in vec3 vertexPosition_modelspace;

        // Values that stay constant for the whole mesh.
        uniform mat4 depthMVP;
        // uniform mat4 model;
        // uniform mat4 view;
        // uniform mat4 projection;

        void main(){
        gl_Position =  /* projection * view * model */ depthMVP * vec4(vertexPosition_modelspace,1.0f);
        })";

        static const char* shadowFragShader =
        R"(#version 330 core

        // Ouput data
        layout(location = 0) out float fragmentdepth;

        void main(){
            // Not really needed, OpenGL does it anyway
            fragmentdepth = gl_FragCoord.z;
        })";
    }
}

#endif