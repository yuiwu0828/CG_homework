#version 330 core

// TODO:
// Implement Toon shading

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 tex;

out vec3 position;
out vec2 texCoord;
out vec3 norm;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	texCoord = tex;
	position = vec3(M * vec4(pos, 1.0));
	norm = mat3(transpose(inverse(M))) * nor;
	gl_Position = P * V * vec4(position, 1.0);
}