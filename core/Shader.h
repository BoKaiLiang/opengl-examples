#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
	
	Shader(const std::string& vertexPath, const std::string& fragPath);

	static Shader* Create(const std::string& vertexPath, const std::string& fragPath);

	~Shader();

	inline GLuint GetID() const { return m_ID; }

	void SetUniformInt(const std::string& uniName, int val) const;
	void SetUniformFloat(const std::string& uniName, float val) const;
	void SetUniformVec2(const std::string& uniName, const glm::vec2& val) const;
	void SetUniformVec3(const std::string& uniName, const glm::vec3& val) const;
	void SetUniformMat3(const std::string& uniName, const glm::mat3& val) const;
	void SetUniformMat4(const std::string& uniName, const glm::mat4& val) const;

private:
	GLuint m_ID;
};

#endif // !_SHADER_H_
