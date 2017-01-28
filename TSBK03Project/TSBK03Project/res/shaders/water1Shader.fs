#version 150

in vec2 outTexCoord;
in vec4 outPosition;

in vec3 toCamera;
in vec3 toSun;

uniform float t, sunExponent;
uniform vec3 sunColor, cameraPosition;
uniform sampler2D normalTex;
uniform sampler2D dudvTex;

float diffuseSun;
float specularSun;
vec3 specularLightSun;
vec3 sunLight;

vec3 reflectedLightSun, reflectedLightFire, reflectedLightStars;
vec2 texCoord;

out vec4 outColor;

void main()
{
	texCoord.x = outTexCoord.x * 1.0;	
	texCoord.y = outTexCoord.y * 1.0;	

	vec2 distortion = 0.01*texture(dudvTex, vec2(texCoord.x + 4.0*t, texCoord.y)).rg;
	vec2 texCoordDist = texCoord + vec2(distortion.x, distortion.y + t);

	vec4 texNormal = texture(normalTex, texCoordDist);
	vec3 waterNormal = vec3(2*texNormal.r - 1, texNormal.b, 2*texNormal.g - 1);
	vec3 normNormal = normalize(waterNormal);

	//LIGHTNING---------------------------------------------------------------------------------------------------
	
	diffuseSun = clamp(dot(normalize(toSun), normalize(normNormal)), 0, 1);
	reflectedLightSun = reflect(-normalize(toSun), normalize(normNormal));
	specularSun = max(dot(reflectedLightSun, normalize(toCamera)), 0);
	specularSun = pow(specularSun, sunExponent);
	specularLightSun = vec3(specularSun);
	sunLight = sunColor * specularLightSun; 	

	//OUTCOLOR----------------------------------------------------------------------------------------------------------------
	outColor = vec4(sunLight, 1.0);
}