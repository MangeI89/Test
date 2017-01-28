#version 330 core

in vec2 outTexCoord;

uniform sampler2D starTex;

out vec4 outColor;

vec2 texCoord1, texCoord2, modOffset, modTexCoord;
vec4 tex1, tex2;

void main()
{
	outColor = texture(starTex, outTexCoord);
}