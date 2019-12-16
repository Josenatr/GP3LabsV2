#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"

class MeshRenderer : public Component
{
private:
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<ShaderProgram> m_program;

public:
	// Inherited via Component
	MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
	~MeshRenderer();
	virtual void OnUpdate(float delatTime) override;
	virtual void OnRender() override;

	
};
