#version 330 core

in vec2 texCoord;
in vec3 norm;
in vec3 position;

uniform sampler2D catTexture;
uniform vec3 camera;
uniform vec3 light;
uniform vec3 la;
uniform vec3 ld;
uniform vec3 ls;
uniform vec3 ma;
uniform vec3 md;
uniform vec3 ms;
uniform float g;

out vec4 fragColor;

void main()
{
	vec4 objColor = texture2D(catTexture, texCoord);
	vec3 ambient = la * ma;
	
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(light - position);
	float tmp = dot(normal, lightDir);
	if(tmp == 1.0) tmp = 1;
	else if(tmp > 0.6) tmp = 0.75;
	else if(tmp > 0.3) tmp = 0.5;
	else if(tmp < 0.0) tmp = 0.0;
	else tmp = 0.25;
	vec3 diffuse = tmp * ld * md;

	vec3 viewDir = normalize(camera - position);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), g) * ls * ms;

	fragColor = vec4((ambient + diffuse + specular), 1.0) * objColor;
}