#pragma once
#include "pch.h"
#include "Common.h"
class Texture
{
private:
	GLuint m_texture;
	GLuint textureHandler;
	std::string m_directory;
	static GLuint Load(const std::string& directory);
public:
	Texture(std::string path);
	Texture();
	~Texture();
	void Bind();
	GLuint loadCubemap(std::vector<std::string> faces);
	GLint getTexHandler() { return textureHandler; }
};
