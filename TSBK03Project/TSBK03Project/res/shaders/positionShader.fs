#version 330 core

in vec2 outTexCoord;

uniform sampler2D velocityTex;
uniform sampler2D positionTex;

uniform sampler2D fireTex;
uniform vec3 cameraForward;
uniform vec2 screenSize;

out vec4 outColor;
vec4 particleInfo;
vec3 position;

void main()
{
	particleInfo = texture(positionTex, vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y));
	position = particleInfo.xyz + 0.02 * (texture(velocityTex, vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y)).rgb - vec3(0.5, 0.5, 0.5));


	if(position.y < 0.5) {
		outColor = vec4(0.5, 0.501, 0.5, 0.0);
		
	} else {
		outColor = vec4(position, clamp(particleInfo.w + 0.006, 0.0, 1.0));
	}
	
}