#version 150

in vec2 outTexCoord;

uniform sampler2D positionTex;
uniform vec2 screenSize;

out vec4 outColor;

void main()
{
	float lifeTime = texture(positionTex, vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y)).a;
	float xMod = 16.0*lifeTime - 4.0 * floor(16.0 * lifeTime / 4.0);
	float xRem = float(trunc(xMod));
	float blend = xMod - xRem;
	float yRem = float(trunc(4.0 * lifeTime));
	
	outColor = vec4(0.25 * xRem,0.25 *  yRem, blend, 1.0);
	
}