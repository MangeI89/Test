#version 330 core

in vec2 outTexCoord;
in float modInstanceID;
in vec2 instID;

uniform sampler2D starTex;
uniform sampler2D fireTex;
uniform sampler2D particleTexCoordInfoTex;
uniform sampler2D particleOffsetInfoTex;

out vec4 outColor;

vec4 tex1, tex2;

void main()
{

	vec4 texCoordOffset = texture(particleTexCoordInfoTex, instID);
	float blendFactor = texture(particleOffsetInfoTex, instID).b;

	vec2 modTexCoord = outTexCoord  / 4.0;

	if(texCoordOffset.y == 1.0) {
		tex1 = vec4(0.0, 0.0, 0.0, 0.0);
		tex2 = vec4(0.0, 0.0, 0.0, 0.0);
	} else {
		tex1 = texture(fireTex, texCoordOffset.xy + modTexCoord);
		tex2 = texture(fireTex, texCoordOffset.zw + modTexCoord);
	}

	outColor = mix(tex1, tex2, blendFactor);
}