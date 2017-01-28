#version 150

in vec2 outTexCoord;
in float height;
in vec4 lightProjectedPos;

in vec3 toEyeView;
in vec3 toSunView;
in vec3 toFireView;
in vec3 toStarsView;
in vec3 outNormal;

vec4 grass, cracked, bottomRock, stoneTerrain, grassDirtTerrain, texColor, texColorDay, texColorNight;
float blendFactor;
vec2 rockTexCoord, grassTexCoord, crackedTexCoord, blendTexCoord;

uniform sampler2D grassTex, rockTex, soilTex, crackedTex, blendTex, terrainRockTex;
uniform sampler2D lightDepthTex;
uniform sampler2D lightDepthTex2;
uniform sampler2D lightDepthTex3;
uniform sampler2D lightDepthTex4;
uniform sampler2D lightDepthTex5;

uniform bool sunOnOff, fireOnOff, starsOnOff;
uniform float t, dayNightBlend, sunExponent, fireExponent, starsExponent;
uniform vec3 cameraPosition, sunColor, fireColor, starsColor;

float diffuseSun, diffuseFire, diffuseStars;
float specularSun, specularFire, specularStars;
vec3 reflectedLightSun, reflectedLightFire, reflectedLightStars;
vec3 fireLight, sunLight, starsLight, totalLight;

out vec4 outColor;

void main()
{	
	//TEXTURES
	grassTexCoord = vec2(outTexCoord.s*8.0, outTexCoord.t*8.0);
	crackedTexCoord = vec2(outTexCoord.s*6.0, outTexCoord.t*6.0);
	blendTexCoord = vec2(outTexCoord.s*1.0, outTexCoord.t*1.0);
	
	grass = texture2D(grassTex, grassTexCoord);
	cracked = texture2D(crackedTex, crackedTexCoord);
	blendFactor = texture2D(blendTex, blendTexCoord).r;

	grassDirtTerrain = mix(grass, cracked, blendFactor);
	stoneTerrain = texture(terrainRockTex, crackedTexCoord);

	if(height > 0) {
		texColor = mix(grassDirtTerrain, stoneTerrain, dayNightBlend);
	} else if( height > -8) {
		rockTexCoord = vec2(outTexCoord.s*(12.0), outTexCoord.t*(12.0));
		bottomRock = texture2D(rockTex, rockTexCoord);

		texColorDay = mix(bottomRock, grassDirtTerrain, (8.0 + height) / 8.0);
		texColorNight = mix(bottomRock, stoneTerrain, (8.0 + height) / 8.0);
		texColor = mix(texColorDay, texColorNight, dayNightBlend);
	} else {
		rockTexCoord = vec2(outTexCoord.s*(12.0), outTexCoord.t*(12.0));
		bottomRock = texture2D(rockTex, rockTexCoord);
		texColor = bottomRock;
	}

	//LIGHTNING---------------------------------------------------------------------------------------------------
	if(sunOnOff) {
		diffuseSun = clamp(dot(normalize(toSunView),normalize(outNormal)),0.0,1.0);
		reflectedLightSun = reflect(-normalize(toSunView), normalize(outNormal));
		specularSun = clamp(dot(reflectedLightSun, normalize(toEyeView)), 0.0, 1.0);
		specularSun = pow(specularSun, sunExponent);
		sunLight = sunColor * vec3(diffuseSun + specularSun);
	} else {
		sunLight = vec3(0.0, 0.0, 0.0);
	}
	
	if(fireOnOff) {
		diffuseFire = clamp(dot(normalize(toFireView),normalize(outNormal)),0,1);
		reflectedLightFire = reflect(-normalize(toFireView), normalize(outNormal));
		specularFire = max(dot(reflectedLightFire, normalize(toEyeView)), 0);
		specularFire = pow(specularFire, fireExponent);
		fireLight = fireColor * vec3(diffuseFire + specularFire);
	} else {
		fireLight = vec3(0.0, 0.0, 0.0);
	}

	if(starsOnOff) {
		diffuseStars = clamp(dot(normalize(toStarsView),normalize(outNormal)),0,1);
		reflectedLightStars = reflect(-normalize(toStarsView), normalize(outNormal));
		specularStars = max(dot(reflectedLightStars, normalize(toEyeView)), 0);
		specularStars = pow(specularStars, starsExponent);
		starsLight = starsColor * vec3(diffuseStars + specularStars);
	} else {
		starsLight = vec3(0.0, 0.0, 0.0);
	}

	totalLight = mix(sunLight, mix(starsLight, fireLight, 0.8), dayNightBlend); //mix(sunLight, starsLight, clamp(dayNightBlend, 0.1, 0.9));

	//SHADOWS-----------------------------------------------------------------------------------------------------------------
	vec3 projectedCoord = lightProjectedPos.xyz / lightProjectedPos.w;
	projectedCoord = projectedCoord/2 + 0.5;

	float xOffset = 1.0 / 1500.0;
	float yOffset = 1.0 / 800.0;
	
	float x = projectedCoord.x; //NO DISTORTION ATM
	float y = projectedCoord.y;

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

	/*
	if(x != 0.0 && y != 0.0 && x != 1.0 && y != 1.0) {
		for (int i = -2; i <= 2; i++) {
			for(int j = -2; j <= 2; j++) {

				vec2 coord = vec2(x + i*xOffset, y + j*yOffset);

				lightDepth = texture(lightDepthTex, coord).r;
				lightDepth2 = texture(lightDepthTex2, coord).r;
				lightDepth3 = texture(lightDepthTex3, coord).r;
				lightDepth4 = texture(lightDepthTex4, coord).r;
				lightDepth5 = texture(lightDepthTex5, coord).r;

				shadow = lightDepth > realDepth-0.00005  ? 1.0 : shadowDampening;
				shadow2 = lightDepth2 > realDepth-0.00005  ? 1.0 : shadowDampening;
				shadow3 = lightDepth3 > realDepth-0.00005  ? 1.0 : shadowDampening;
				shadow4 = lightDepth4 > realDepth-0.00005  ? 1.0 : shadowDampening;
				shadow5 = lightDepth5 > realDepth-0.00005  ? 1.0 : shadowDampening;
				
				averageShadow = averageShadow + shadow + shadow2 + shadow3 + shadow4 + shadow5;
			}
		}
		averageShadow = averageShadow / (125.0);
	} else {
		averageShadow = 1.0;
	}*/
	
	if(x > 0.0 && x < 1.0 && y > 0.0 && y < 1.0) {
	
		vec2 coord = vec2(x, y);

		lightDepth = texture(lightDepthTex, coord).r;
		shadow = lightDepth > realDepth-0.05  ? 1.0 : shadowDampening;

		
		lightDepth2 = texture(lightDepthTex2, coord).r;
		lightDepth3 = texture(lightDepthTex3, coord).r;
		lightDepth4 = texture(lightDepthTex4, coord).r;
		lightDepth5 = texture(lightDepthTex5, coord).r;

		shadow2 = lightDepth2 > realDepth-0.05  ? 1.0 : shadowDampening;
		shadow3 = lightDepth3 > realDepth-0.05  ? 1.0 : shadowDampening;
		shadow4 = lightDepth4 > realDepth-0.05  ? 1.0 : shadowDampening;
		shadow5 = lightDepth5 > realDepth-0.05  ? 1.0 : shadowDampening;
				
		averageShadow = (shadow + shadow2 + shadow3 + shadow4 + shadow5) / 5;
		//averageShadow = shadow;
	} else {
		averageShadow = 1.0;
	}

	//outColor = mix(vec4(vec3(averageShadow), 1.0), texColor, 0.5);
	//outColor = diffuseLightMain * texColor;
	outColor = vec4(totalLight, 1.0) * texColor;
}