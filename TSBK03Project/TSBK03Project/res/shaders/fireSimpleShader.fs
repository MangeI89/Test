#version 330 core

in vec2 offset;
in float blendFactor;
in vec2 outTexCoord;

uniform sampler2D fireTex;

out vec4 outColor;

vec2 texCoord1, texCoord2;
vec4 tex1, tex2;

void main()
{
	
	texCoord1 = vec2(offset.x * 0.25, offset.y * 0.25) + outTexCoord / 4.0f;
	tex1 = texture(fireTex, texCoord1);
	if(offset.y > 3.5) {
		tex1 = vec4(0.0, 0.0, 0.0, 0.0);
		tex2 = tex1;

	} else if(offset.x > 2.5 && offset.y > 2.5) {
		tex2 = tex1;
	} else if(offset.x > 2.5){
		texCoord2 = vec2(0.0, (offset.y + 1.0) * 0.25) + outTexCoord / 4.0f;
		tex2 = texture(fireTex, texCoord2);
	} else {
		texCoord2 = texCoord1 + vec2(0.25, 0.0);
		tex2 = texture(fireTex, texCoord2);
	}

	outColor = mix(tex1, tex2, blendFactor);

}