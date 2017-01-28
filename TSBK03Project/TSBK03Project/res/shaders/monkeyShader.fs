#version 150

in vec2 outTexCoord;
uniform sampler2D brickTex;

out vec4 outColor;

void main()
{
	outColor = texture2D(brickTex, outTexCoord);
}