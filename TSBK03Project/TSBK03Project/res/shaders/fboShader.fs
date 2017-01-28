#version 150

in vec2 outTexCoord;
uniform sampler2D lightDepthTex;
uniform sampler2D waterDepthTex;

out vec4 outColor;

void main()
{
	//outColor = vec4(0.0, 0.0, 1.0, 1.0);
	//outColor = texture(lightDepthTex, outTexCoord);
	outColor = texture(lightDepthTex, outTexCoord);
}