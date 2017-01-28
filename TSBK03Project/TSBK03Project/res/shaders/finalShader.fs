#version 150

in vec2 outTexCoord;
uniform sampler2D sunPostProcessingTex;
uniform sampler2D environmentTex;

float contrast = 0.3;
out vec4 outColor;

void main()
{
	outColor = texture(environmentTex, outTexCoord);
	outColor.rgb = (outColor.rgb - 0.5) * (1.0 + contrast) + 0.5;
}