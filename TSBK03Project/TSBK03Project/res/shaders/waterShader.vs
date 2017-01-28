#version 150

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec4 outPosition;
out vec2 outTexCoord;

out vec3 toCamera;
out vec3 toSun;
out vec3 toFire;
out vec3 toStars;
out vec4 lightProjectedPos;

out float height;

uniform vec3 cameraPosition, sunPosition, firePosition, starsPosition;
uniform mat4 model, camera, projection, lightMatrix;

vec3 worldLight;

void main()
{
	mat3 normalMatrix = mat3(camera * transpose(inverse(model)));

	vec4 worldPosition = model * vec4(position, 1.0);

	toCamera = cameraPosition - worldPosition.xyz;
	toSun = sunPosition - worldPosition.xyz;
	toFire = firePosition - worldPosition.xyz;
	toStars = starsPosition - worldPosition.xyz;

	outTexCoord = 8.0 * texCoord;
	outPosition = projection * camera * worldPosition;

	lightProjectedPos = lightMatrix * worldPosition;
	gl_Position = outPosition;
}