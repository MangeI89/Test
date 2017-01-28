#version 150

in vec2 outTexCoord;

uniform sampler2D positionTex;

out vec4 outColor;

void main()
{
	
	outColor = texture(positionTex, outTexCoord);

}