#version 330 core

in vec2 texCoord;
in vec3 norm;
in vec3 position;

uniform sampler2D catTexture;
uniform vec3 camera;

out vec4 fragColor;

void main()
{
	vec4 objColor = texture2D(catTexture, texCoord);
	vec3 normal = normalize(norm);
	vec3 viewDir = normalize(camera - position);
	if(dot(normal, viewDir) <= 0.2) fragColor = vec4(0.0, 0.0, 1.0, 1.0);
	else fragColor = vec4(0.0, 0.0, 0.0, 1.0);
}