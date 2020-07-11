#include "Model.h"

#include <iostream>

#include "tiny_obj_loader.h"

glm::vec3 GetVertex(const tinyobj::attrib_t& attrib, int index) {
	return glm::vec3(
		attrib.vertices[3 * index + 0],
		attrib.vertices[3 * index + 1],
		attrib.vertices[3 * index + 2]);
}

glm::vec4 GetColor(const tinyobj::attrib_t& attrib, int index) {
	return glm::vec4(
		attrib.colors[3 * index + 0],
		attrib.colors[3 * index + 1],
		attrib.colors[3 * index + 2],
		1.0);
}

glm::vec3 GetNormal(const tinyobj::attrib_t& attrib, int index) {
	return glm::vec3(
		attrib.normals[3 * index + 0],
		attrib.normals[3 * index + 1],
		attrib.normals[3 * index + 2]);
}

glm::vec2 GetTexCoords(const tinyobj::attrib_t& attrib, int index) {
	return glm::vec2(
		attrib.texcoords[2 * index],
		1.0f - attrib.texcoords[2 * index + 1]);
}

Model* Model::LoadObjFile(const std::string& objPath)
{
	Model* model = new Model();

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str());

	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}
	if (!err.empty()) {
		std::cerr << "ERROE: " << err << std::endl;
		exit(EXIT_FAILURE);
	}

	for (size_t s = 0; s < shapes.size(); s++) {

		std::string name = shapes[s].name;

		Mesh mesh;

		std::map<int, tinyobj::index_t> unique_indices;
		std::map<int, tinyobj::index_t>::iterator iter;

		int mi = -1;
		int mCounter = 0;
		unsigned int iCounter = 0;
		for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
			int f = (int)floor(i / 3);

			tinyobj::index_t index = shapes[s].mesh.indices[i];
			int vi = index.vertex_index;
			int ni = index.normal_index;
			int ti = index.texcoord_index;

			bool reuse = false;
			iter = unique_indices.find(vi);

			// if already exist 
			if (iter != unique_indices.end())
				// and have the same attributes
				if ((iter->second.normal_index == ni) &&
					(iter->second.texcoord_index == ti))
					reuse = true;

			// Re use the vertex
			if (reuse)
				mesh.AddIndex((unsigned int)iter->second.vertex_index);
			// Other wise create a new one
			else {
				unique_indices[vi].vertex_index = (int)iCounter;
				unique_indices[vi].normal_index = ni;
				unique_indices[vi].texcoord_index = ti;

				mesh.AddVertex(GetVertex(attrib, vi));
				mesh.AddColor(GetColor(attrib, vi));

				// If there is normals add them
				if (attrib.normals.size() > 0)
					mesh.AddNormal(GetNormal(attrib, ni));

				// If there is texcoords add them
				if (attrib.texcoords.size() > 0)
					mesh.AddTexCoord(GetTexCoords(attrib, ti));

				mesh.AddIndex(iCounter++);
			}
		}

		mesh.ComputeTangents();

		mesh.Setup();

		model->m_Meshes.insert({ name, mesh });
	}

	return model;
}

void Model::Render()
{
	for (auto mesh : m_Meshes)
	{
		mesh.second.Draw();
	}
}
