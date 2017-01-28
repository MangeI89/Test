#version 330 core

in vec2 outTexCoord;
uniform sampler2D starTex;
out vec4 outColor;

void main()
{
	outColor = texture(starTex, outTexCoord);
}