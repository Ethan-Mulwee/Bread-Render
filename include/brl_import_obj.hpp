#ifndef BRL_IMPORT_OBJ
#define BRL_IMPORT_OBJ

#include "brl_vertexbuffer.hpp"

namespace brl {
    MeshData parse_obj(const char* path);

    Mesh import_obj(const char* path);
}

#endif