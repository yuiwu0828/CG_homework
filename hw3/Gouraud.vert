#version 330 core

// TODO:
// Implement Gouraud shading

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 tex;

out vec2 texCoord;
out vec3 phongColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 camera;
uniform vec3 light;
uniform vec3 la;
uniform vec3 ld;
uniform vec3 ls;
uniform vec3 ma;
uniform vec3 md;
uniform vec3 ms;
uniform float g;

void main()
{
	texCoord = tex;
	vec3 position = vec3(M * vec4(pos, 1.0));
	vec3 norm = mat3(transpose(inverse(M))) * nor;
	gl_Position = P * V * vec4(position, 1.0);

	vec3 ambient = la * ma;
	
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(light - position);
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * ld * md;

	vec3 viewDir = normalize(camera - position);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), g) * ls * ms;

	phongColor = ambient + diffuse + specular;
}