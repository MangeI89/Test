#version 150

in vec2 outTexCoord;

uniform sampler2D velocityTex;
uniform sampler2D positionTex;
uniform vec2 screenSize;

out vec4 outColor;
vec4 currentVelocity;

void main()
{	
	currentVelocity = texture(velocityTex, vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y));

	if(texture(positionTex, vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y)).a == 0.0) {
		
		outColor = vec4(currentVelocity.x, currentVelocity.w, currentVelocity.z, currentVelocity.w);
		
	} else {
		outColor = vec4(currentVelocity.x, currentVelocity.y - 0.015, currentVelocity.z, currentVelocity.w);
	}
	
	
	//outColor = vec4(0.0, velocity, 0.0, 1.0);
	//outColor = texture2D(fireTex, outTexCoord);
}