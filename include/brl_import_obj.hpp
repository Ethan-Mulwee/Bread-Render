#ifndef BRL_IMPORT_OBJ
#define BRL_IMPORT_OBJ

#include "brl_vertexbuffer.hpp"

namespace brl {
    MeshData parseObj(const char* path);

    Mesh importObj(const char* path);
}

#endif