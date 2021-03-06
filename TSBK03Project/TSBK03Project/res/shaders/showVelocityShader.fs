#version 150

in vec2 outTexCoord;
uniform sampler2D velocityTex;

out vec4 outColor;

void main()
{
	outColor = vec4(texture(velocityTex, outTexCoord).rgb, 1.0);
	//outColor = vec4(1.0, 1.0, 1.0, 1.0);
}