#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Core.h"

struct Material {
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;
	float Shininess;
	Shader shader;
};

#endif // _MATERIAL_H_