#include "Shader.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "DebugLog.h"

static std::string ReadFileAsString(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize((size_t)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		DEBUG_ERROR("Cannot open the file: {0}", filepath);
	}

	return result;
}

static GLuint compileShader(GLenum type, const std::string& source)
{
	GLuint shader = glCreateShader(type);

	const GLchar* sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, 0);

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(shader);

		DEBUG_ERROR("{0}", infoLog.data());
	}

	return shader;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragPath)
{
	std::string vertexSource = ReadFileAsString(vertexPath);
	std::string fragmentSource = ReadFileAsString(fragPath);

	m_ID = glCreateProgram();
	int glShaderIDIndex = 0;

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	glAttachShader(m_ID, vertexShader);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
	glAttachShader(m_ID, fragmentShader);

	glLinkProgram(m_ID);

	GLint isLinked = 0;
	glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(m_ID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		DEBUG_ERROR("{0}", infoLog.data());
	}

	glDetachShader(m_ID, vertexShader);
	glDetachShader(m_ID, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader* Shader::Create(const std::string& vertexPath, const std::string& fragPath)
{
	return new Shader(vertexPath, fragPath);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::SetUniformInt(const std::string& uniName, int val) const
{
	glUseProgram(m_ID);
	glUniform1i(glGetUniformLocation(m_ID, uniName.c_str()), val);
}

void Shader::SetUniformFloat(const std::string& uniName, float val) const
{
	glUseProgram(m_ID);
	glUniform1f(glGetUniformLocation(m_ID, uniName.c_str()), val);
}

void Shader::SetUniformVec2(const std::string& uniName, const glm::vec2& val) const
{
	glUseProgram(m_ID);
	glUniform2fv(glGetUniformLocation(m_ID, uniName.c_str()), 1, &val[0]);
}

void Shader::SetUniformVec3(const std::string& uniName, const glm::vec3& val) const
{
	glUseProgram(m_ID);
	glUniform3fv(glGetUniformLocation(m_ID, uniName.c_str()), 1, &val[0]);
}

void Shader::SetUniformMat3(const std::string& uniName, const glm::mat3& val) const
{
	glUseProgram(m_ID);
	glUniformMatrix3fv(glGetUniformLocation(m_ID, uniName.c_str()), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniformMat4(const std::string & uniName, const glm::mat4 & val) const
{
	glUseProgram(m_ID);
	glUniformMatrix4fv(glGetUniformLocation(m_ID, uniName.c_str()), 1, GL_FALSE, &val[0][0]);
}
