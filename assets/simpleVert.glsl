#version 420
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 texCoords;
layout(location = 3) in vec3 normal;

uniform mat4 MVP;
uniform mat4 model;

out vec3 FragPos;
out vec2 texCoords0;
out vec3 normal0;

void main()
{
	gl_Position = MVP * vec4(pos, 1.0f);
	texCoords0 = texCoords;
	normal0 = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(pos, 1.0f));
}