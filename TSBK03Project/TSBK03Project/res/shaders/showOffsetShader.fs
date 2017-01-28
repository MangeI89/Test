#version 150

in vec2 outTexCoord;
uniform sampler2D particleOffsetInfoTex;

out vec4 outColor;

void main()
{
	outColor = vec4(texture(particleOffsetInfoTex, outTexCoord).x, 0.0, 0.0, 1.0);
	//outColor = vec4(1.0, 1.0, 1.0, 1.0);
}