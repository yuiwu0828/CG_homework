#version 330 core

in vec3 phongColor;
in vec2 texCoord;

uniform sampler2D catTexture;

out vec4 fragColor;

void main()
{
	vec4 objColor = texture(catTexture, texCoord);
	fragColor = vec4(phongColor , 1.0) * objColor;
}