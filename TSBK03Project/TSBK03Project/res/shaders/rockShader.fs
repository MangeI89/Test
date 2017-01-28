#version 150

in vec2 outTexCoord;
uniform sampler2D rockNormalTex, stoneTex;

vec4 tex1, tex2, tex3;
out vec4 outColor;

void main()
{
	tex1 = texture2D(rockNormalTex, outTexCoord);
	tex2 = texture2D(stoneTex, outTexCoord);

	outColor = tex2;
}