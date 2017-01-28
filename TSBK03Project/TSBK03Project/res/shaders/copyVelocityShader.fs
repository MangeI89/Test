#version 150

in vec2 outTexCoord;

uniform sampler2D velocityTex;

out vec4 outColor;

void main()
{
	
	outColor = texture(velocityTex, outTexCoord);

}