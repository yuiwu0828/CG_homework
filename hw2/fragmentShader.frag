#version 330 core
// TO DO:
// Implement fragment shader

in vec2 texCoord;

uniform sampler2D catTexture;
uniform sampler2D boxTexture;

uniform float choose;
uniform float D;
uniform bool on;
uniform bool c;

uniform float k1;
uniform float k2;
uniform float k3;

out vec4 fragColor; 

void main()
{
	vec4 tmp = texture(catTexture, texCoord);
	if(tmp.r < 0.6f && tmp.g < 0.3f && on){
		tmp.r = 0.0;
		tmp.g = 0.0;
		tmp.b = 0.0;
	}
	if(c) tmp = tmp + vec4(k1, k2, k3, 0.0);
	fragColor = mix(tmp, texture(boxTexture, texCoord), choose) * vec4(D, D, D, 1.0) ;
	
}
