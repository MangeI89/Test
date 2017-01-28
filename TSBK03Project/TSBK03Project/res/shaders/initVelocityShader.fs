#version 150

in vec2 outTexCoord;
uniform vec2 screenSize;

out vec4 outColor;

void main()
{	
	vec2 fragTexCoord = vec2(gl_FragCoord.x / screenSize.x, gl_FragCoord.y / screenSize.y);
	vec2 seedX = vec2(sin(9637.345 * fragTexCoord.x), cos(9637.345 * fragTexCoord.y));
	vec2 seedY = vec2(sin(7234.872 * fragTexCoord.x), cos(7234.872 * fragTexCoord.y));
	vec2 seedZ = vec2(sin(23674.872 *fragTexCoord.x), cos(23674.872 * fragTexCoord.y));
    	float randNrX = fract(sin(dot(seedX ,vec2(12.9898,78.233))) * 43758.5453); 
	float randNrY = fract(sin(dot(seedY ,vec2(12.9898,78.233))) * 43758.5453); 
	float randNrZ = fract(sin(dot(seedZ ,vec2(12.9898,78.233))) * 43758.5453); 
	outColor = vec4(randNrX, randNrY / 2.0 + 1.5, randNrZ, randNrY / 2.0 + 1.5);
}