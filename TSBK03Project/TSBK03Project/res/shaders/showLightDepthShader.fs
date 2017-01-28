#version 150

in vec2 outTexCoord;
uniform sampler2D lightDepthTex;

out vec4 outColor;

void main()
{
	outColor = texture(lightDepthTex, outTexCoord);
	//outColor = vec4(1.0, 1.0, 1.0, 1.0);
}