#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"
#include "Lighting.h"

MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_model = model;

	m_program = program;
	m_texture = texture;
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	//LOG_DEBUG("I'm Updating!");
}
void MeshRenderer::OnRender()
{
	m_program->Use();

	//set uniforms here!
	glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();
	
	GLuint loc = glGetUniformLocation(m_program->Get(), "model");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(model));
	
	/*glm::vec3 viewPos = Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition();
	loc = glGetUniformLocation(m_program->Get(), "viewPos");
	glUniform3f(loc, viewPos.x, viewPos.y, viewPos.z);*/
	
	glm::mat4 mvp = Application::GetInstance()->GetCamera()->GetProj() * Application::GetInstance()->GetCamera()->GetView() * model;
	loc = glGetUniformLocation(m_program->Get(), "MVP");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));
	
	int gamma = 0.5;
	GLint applyGamma = gamma;
	loc = glGetUniformLocation(m_program->Get(), "gamma");
	glUniform1i(loc, applyGamma);

	/*glm::vec3 lightColor = glm::vec3(0.03f, 0.45f, 0.32f);
	loc = glGetUniformLocation(m_program->Get(), "lightColor");
	glUniform3f(loc, lightColor.x, lightColor.y, lightColor.z);
	
	glm::vec3 lightPosit = glm::vec3(10.f, 0.f, -40.f);
	loc = glGetUniformLocation(m_program->Get(), "lightPos");
	glUniform3f(loc, lightPosit.x, lightPosit.y, lightPosit.z);*/


	m_texture->Bind();

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();

		if (m_texture) m_texture->Bind();

		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));

	}




}
