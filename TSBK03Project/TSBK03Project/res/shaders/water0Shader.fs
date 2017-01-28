#version 150

in vec2 outTexCoord;

in vec3 toCamera;
in vec3 toSun;

uniform float sunExponent;
uniform vec3 sunColor, cameraPosition;

float diffuseSun;
float specularSun;
vec3 specularLightSun;
vec3 sunLight;

vec3 reflectedLightSun, reflectedLightFire, reflectedLightStars;
vec2 texCoord;

out vec4 outColor;

void main()
{

	//LIGHTNING---------------------------------------------------------------------------------------------------
	
	diffuseSun = clamp(dot(normalize(toSun), vec3(0.0, 1.0, 0.0)), 0, 1);
	reflectedLightSun = reflect(-normalize(toSun), vec3(0.0, 1.0, 0.0));
	specularSun = max(dot(reflectedLightSun, normalize(toCamera)), 0);
	specularSun = pow(specularSun, sunExponent);
	specularLightSun = vec3(specularSun);
	sunLight = sunColor * specularLightSun; 	
	
	//OUTCOLOR----------------------------------------------------------------------------------------------------------------
	outColor = vec4(sunLight, 1.0);
}