#version 150

in vec2 outTexCoord;
in vec4 outPosition;

in vec3 toCamera;
in vec3 toSun;
in vec3 toFire;
in vec3 toStars;
in vec4 lightProjectedPos;

uniform sampler2D reflectionTex;
uniform sampler2D refractionTex;
uniform sampler2D lightDepthTex;
uniform sampler2D lightDepthTex2;
uniform sampler2D lightDepthTex3;
uniform sampler2D lightDepthTex4;
uniform sampler2D lightDepthTex5;

uniform sampler2D waterDepthTex;

uniform sampler2D normalTex;
uniform sampler2D dudvTex;

uniform bool sunOnOff, fireOnOff, starsOnOff;
uniform float t, near, far, sunExponent, fireExponent, starsExponent, dayNightBlend;
uniform vec3 cameraPosition, sunColor, fireColor, starsColor;

float diffuseSun, diffuseFire, diffuseStars;
float specularSun, specularFire, specularStars;
vec3 specularLightSun, specularLightFire, specularLightStars;
vec3 sunLight, fireLight, starsLight, totalLight;

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
	
	if(sunOnOff) {
		diffuseSun = clamp(dot(normalize(toSun),normalize(normNormal)),0,1);
		reflectedLightSun = reflect(-normalize(toSun), normalize(normNormal));
		specularSun = max(dot(reflectedLightSun, normalize(toCamera)), 0);
		specularSun = pow(specularSun, sunExponent);
		specularLightSun = vec3(specularSun);
		sunLight = sunColor * specularLightSun; 	

	} else {
		sunLight = vec3(0.0, 0.0, 0.0);
	}

	if(fireOnOff) {
		diffuseFire = clamp(dot(normalize(toFire),normalize(normNormal)),0,1);
		reflectedLightFire = reflect(-normalize(toFire), normalize(normNormal));
		specularFire = max(dot(reflectedLightFire, normalize(toCamera)), 0); //Make light as in terrainShader!!!!
		specularFire = pow(specularFire, fireExponent);
		specularLightFire = vec3(specularFire);	
		fireLight = fireColor * vec3(specularFire); //NO DIFFUSE LIGHT AT THE MOMENT
	} else {
		fireLight = vec3(0.0, 0.0, 0.0);
	}

	if(starsOnOff) {
		diffuseStars = clamp(dot(normalize(toStars),normalize(normNormal)),0,1);
		reflectedLightStars = reflect(-normalize(toStars), normalize(normNormal));
		specularStars = max(dot(reflectedLightStars, normalize(toCamera)), 0); //Make light as in terrainShader!!!!
		specularStars = pow(specularStars, starsExponent);
		specularLightStars = vec3(specularStars);	
		starsLight = starsColor * vec3(specularStars); //NO DIFFUSE LIGHT AT THE MOMENT
	} else {
		starsLight = vec3(0.0, 0.0, 0.0);
	}

	totalLight = mix(sunLight, mix(starsLight, fireLight, 0.8), dayNightBlend); //mix(sunLight, fireLight, clamp(dayNightBlend, 0.1, 0.9));
	//REFLECTION AND REFRACTION-----------------------------------------------------------------------------------------------
	vec2 reflectionCoord = vec2((outPosition.x/outPosition.w)/2 + 0.5, (outPosition.y/outPosition.w)/2 + 0.5);

	
	float depth = texture(waterDepthTex, reflectionCoord).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	float waterDepth = floorDistance - waterDistance;

	reflectionCoord = reflectionCoord + distortion;
	reflectionCoord.x = clamp(reflectionCoord.x, 0.001, 0.999);
	reflectionCoord.y = clamp(reflectionCoord.y, 0.001, 0.999);
	vec4 reflection = texture(reflectionTex, reflectionCoord);
	vec4 refraction = texture(refractionTex, reflectionCoord);

	float refractionFactor = dot(normalize(toCamera), vec3(0,1,0));
	refractionFactor = pow(refractionFactor, 6.0);

	vec4 environment = mix(reflection, refraction, refractionFactor);

	//SHADOWS-----------------------------------------------------------------------------------------------------------------
	vec3 projectedCoord = lightProjectedPos.xyz / lightProjectedPos.w;
	projectedCoord = projectedCoord/2 + 0.5;

	float xOffset = 1.0 / 1500.0;
	float yOffset = 1.0 / 800.0;
	
	float x = projectedCoord.x + distortion.x;
	float y = projectedCoord.y + distortion.y;

	float averageShadow = 0;
	float realDepth = projectedCoord.z;

	float shadowDampening = 0.5;

	float lightDepth;
	float lightDepth2;
	float lightDepth3;
	float lightDepth4;
	float lightDepth5;

	float shadow;
	float shadow2;
	float shadow3;
	float shadow4;
	float shadow5;
	
	if(x > 0.0 && x < 1.0 && y > 0.0 && y < 1.0) {
	
		vec2 coord = vec2(x, y);

		lightDepth = texture(lightDepthTex, coord).r;
		shadow = lightDepth > realDepth-0.000005  ? 1.0 : shadowDampening;

		
		lightDepth2 = texture(lightDepthTex2, coord).r;
		lightDepth3 = texture(lightDepthTex3, coord).r;
		lightDepth4 = texture(lightDepthTex4, coord).r;
		lightDepth5 = texture(lightDepthTex5, coord).r;

		shadow2 = lightDepth2 > realDepth-0.005  ? 1.0 : shadowDampening;
		shadow3 = lightDepth3 > realDepth-0.005  ? 1.0 : shadowDampening;
		shadow4 = lightDepth4 > realDepth-0.005  ? 1.0 : shadowDampening;
		shadow5 = lightDepth5 > realDepth-0.005  ? 1.0 : shadowDampening;
				
		averageShadow = (shadow + shadow2 + shadow3 + shadow4 + shadow5) / 5;
	} else {
		averageShadow = 1.0;
	}
	
	//OUTCOLOR----------------------------------------------------------------------------------------------------------------
	outColor = vec4(vec3(averageShadow), 1.0) * environment +  vec4(totalLight, 0.0);
	outColor.a = clamp(waterDepth / 7.0, 0.0, 1.0);
}