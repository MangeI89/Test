#version 150

in vec2 outTexCoord;
uniform sampler2D positionTex;

out vec4 outColor;

void main()
{
	outColor = vec4(texture(positionTex, outTexCoord).rgb, 1.0);
	//outColor = vec4(1.0, 1.0, 1.0, 1.0);
}