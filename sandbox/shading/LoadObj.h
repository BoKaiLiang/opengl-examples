#ifndef _LOAD_OBJ_H_
#define _LOAD_OBJ_H_

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "tiny_obj_loader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

void LoadObjFile(const std::string& objPath, std::vector<Vertex>& vertices);

#endif // _LOAD_OBJ_H_