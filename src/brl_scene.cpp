#include "brl_scene.hpp"

namespace brl {
    Mesh createMesh(MeshData* meshData) {
        return Mesh{createVertexbuffer(meshData)};
    }
}