#ifndef BRL_SCENEDATA
#define BRL_SCENEDATA

#include "brl_vertexbuffer.hpp"
#include <vector>


namespace brl {
    
    struct Mesh {
        Vertexbuffer buffer;
    };
    
    Mesh createMesh(MeshData* meshData);
    
    struct Model {
        Mesh mesh;
        smath::matrix4x4 transform;
        smath::vector4 color = smath::vector4{1.0f, 1.0f, 1.0f, 1.0f};
    };

    struct SceneData {
        std::vector<Model> models;
    };
}

#endif