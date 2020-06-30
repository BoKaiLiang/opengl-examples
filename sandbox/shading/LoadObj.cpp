#include "LoadObj.h"

#include "DebugLog.h"

void LoadObjFile(const std::string& objPath, std::vector<Vertex>& vertices)
{
    tinyobj::attrib_t attribute;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string warn_msg, err_msg;
    if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warn_msg, &err_msg, objPath.c_str()))
    {
        DEBUG_ERROR("Cannot open the .obj file: {0}", objPath);
        exit(EXIT_FAILURE);
    }

    if (!warn_msg.empty()) {
        DEBUG_WARN("{0}", warn_msg);
    }

    if (!err_msg.empty()) {
        DEBUG_ERROR("{0}", err_msg);
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {

        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                Vertex vertex{ };

                size_t pos_idx  = 3 * idx.vertex_index;
                size_t norm_idx = 3 * idx.normal_index;
                size_t uv_idx   = 2 * idx.texcoord_index;

                vertex.Position = {
                    attribute.vertices[pos_idx + 0],
                    attribute.vertices[pos_idx + 1],
                    attribute.vertices[pos_idx + 2]
                };

                vertex.Normal = {
                    attribute.normals[norm_idx + 0],
                    attribute.normals[norm_idx + 1],
                    attribute.normals[norm_idx + 2]
                };

                vertex.TexCoord = {
                    attribute.texcoords[uv_idx + 0],
                    1 - attribute.texcoords[uv_idx + 1]
                };


                vertices.push_back(vertex);
            }
            index_offset += fv;
        }
    }
}