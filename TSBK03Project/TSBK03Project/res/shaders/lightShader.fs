#version 150

in vec2 outTexCoord;

out vec4 outColor;
uniform sampler2D fireTex, dudvTex, sunTex;
uniform float t;

vec2 radialDir, radialDirMod, texCoord;
vec2 offset;
float edgeDistance;

void main()
{
	vec2 distortion = 0.1*texture(dudvTex, vec2(3*outTexCoord.x + 4.0*t, 3*outTexCoord.y)).rg - 0.05;
	radialDir = outTexCoord - 0.5;
	texCoord = outTexCoord / 4.0;
	radialDirMod = radialDir / 4.0;
	vec4 colorSum = vec4(0.0, 0.0, 0.0, 0.0);
	edgeDistance = sqrt(pow(radialDir.x, 2) + pow(radialDir.y, 2));

	if(edgeDistance > 0.48) {
		colorSum = vec4(0.0, 0.0, 0.0, 0.0);
	} else {
		for(float i = -4.0; i < 5.0; i++) {
		
			offset.x = clamp(texCoord.x + radialDirMod.x * (i * 0.05), 0.0, 1.0);
			offset.y = clamp(texCoord.y + radialDirMod.y * (i * 0.05), 0.0, 1.0);
			colorSum += texture(fireTex, offset);
		}
		colorSum /= 9.0f;
	}
	
	outColor = colorSum;
	//outColor = texture(fireTex, texCoord);
	//outColor = texture(fireTex, vec2(0.75, 0.25) + outTexCoord / 4.0);
}