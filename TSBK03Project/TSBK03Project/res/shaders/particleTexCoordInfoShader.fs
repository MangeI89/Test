#version 150

in vec2 outTexCoord;

uniform sampler2D particleOffsetInfoTex;
uniform vec2 screenSize;

out vec4 outColor;
vec2 texCoord1, texCoord2;

void main()
{
	vec2 offsetInfo = texture(particleOffsetInfoTex, vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y)).rg;

	texCoord1 = vec2(offsetInfo.x, offsetInfo.y);

	if(offsetInfo.x == 0.75 && offsetInfo.y == 0.75) {
	 	texCoord2 = texCoord1;
		
	} else if(offsetInfo.x == 0.75) {
		texCoord2 = vec2(0.0, offsetInfo.y + 0.25);

	} else {
		texCoord2 = texCoord1 + vec2(0.25, 0.0);
	}

	outColor = vec4(texCoord1, texCoord2);
}