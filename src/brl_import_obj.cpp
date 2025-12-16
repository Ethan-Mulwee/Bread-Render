#include "brl_import_obj.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

namespace brl {

    // Code adapted from https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    MeshData parse_obj(const char* path) {
        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<smath::vector3> temp_vertices;
        std::vector<smath::vector2> temp_uvs;
        std::vector<smath::vector3> temp_normals;

        MeshData mesh;

        FILE* file = fopen(path, "r");
        if (file == NULL) {
            std::cout << "Error opening file \n";
            return MeshData{};
        }

        while(true) {
            char lineHeader[128];
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break;
            
            if (strcmp(lineHeader, "v") == 0) {
                smath::vector3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            } else if (strcmp(lineHeader, "vt") == 0) {
                smath::vector2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y);
                temp_uvs.push_back(uv);
            } else if (strcmp(lineHeader, "vn") == 0) {
                smath::vector3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            } else if (strcmp(lineHeader, "f") == 0) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(
                    file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                    &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                    &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                    &vertexIndex[2], &uvIndex[2], &normalIndex[2]
                );
                if (matches != 9) {
                    std::cout << "Parsing error \n";
                    return MeshData{};
                }

                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);

            }
        }
        for (int i = 0; i < vertexIndices.size(); i++) {
            unsigned int vertexIndex = vertexIndices[i];
            unsigned int normalIndex = normalIndices[i];
            smath::vector3 position = temp_vertices[vertexIndex-1];
            smath::vector3 normal = temp_normals[normalIndex-1];

            Vertex vertex;
            vertex.position = position;
            vertex.normal = normal;
            mesh.vertices.push_back(vertex);
            mesh.indices.push_back(i);
        }

        return mesh;
    }

    Mesh import_obj(const char *path) {
        MeshData data = parse_obj(path);
        return Mesh{create_vertexbuffer(&data)};
    }
}