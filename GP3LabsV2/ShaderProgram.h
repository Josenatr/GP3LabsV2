#pragma once
#include "pch.h"
#include "Shader.h"
class ShaderProgram
{
private:
	//TODO:Track currently active shader
	GLuint m_program;
	Shader* m_vShader, * m_fShader;
	bool CheckForLinkErrors();
public:
	ShaderProgram(const std::string& vShaderFile, const std::string& fShaderFile);
	~ShaderProgram();
	void BindAttribLoc(GLuint loc, const char* name);
	void Link();
	void Use();
	GLuint Get() { return m_program; }

	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(m_program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_program, name.c_str()), x, y, z);

		if ((glGetUniformLocation(m_program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(m_program, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
};