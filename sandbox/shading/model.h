#ifndef _MODEL_H_
#define _MODEL_H_

#include <unordered_map>
#include <string>

#include "mesh.h"

class Model
{
public:
	Model() = default;
	~Model() = default;

	static Model* LoadObjFile(const std::string& objPath);

	void Render();

private:
	std::unordered_map<std::string, Mesh> m_Meshes;
};

#endif // _MODEL_H_