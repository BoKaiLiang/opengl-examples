#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Core.h"

struct Light
{
	Light(glm::vec3 pos, 
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), 
		glm::vec3 ambient = glm::vec3(0.8f, 0.8f, 0.8f));

	~Light() = default;

	glm::vec3 Position;
	glm::vec3 LightColor;
	glm::vec3 Ambient;
};

#endif // _LIGHT_H_