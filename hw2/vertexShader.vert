#version 330 core
// TO DO:
// Implement vertex shader
// note: remember to set gl_Position

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 tex;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform float f1;
uniform bool up;

out vec2 texCoord;

void main()
{
	vec3 tmp = pos;
	if(up) tmp = pos * vec3(1.0, f1, 1.0);
	texCoord = tex;
	gl_Position = P * V * M * vec4(tmp, 1.0f);
	
}