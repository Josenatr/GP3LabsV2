#pragma once
#include "pch.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "Resources.h"

class Lighting
{
private:
	glm::vec3 lightingElement = glm::vec3(0.0f, 0.0f, 0.0f);
	std::shared_ptr<ShaderProgram> shader;
public:
	void passLightUniform() {
		shader = Resources::GetInstance()->GetShader("simple");
		GLuint loc = glGetUniformLocation(shader->Get(), "lightPos");
		glUniform3f(loc, lightingElement.x, lightingElement.y, lightingElement.z);
	}

};