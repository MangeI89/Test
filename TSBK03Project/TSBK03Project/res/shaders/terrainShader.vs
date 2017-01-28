#version 150

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec3 outNormal;
out vec2 outTexCoord;
out float height;
out vec4 lightProjectedPos;

out vec3 toSunView;
out vec3 toFireView;
out vec3 toStarsView;
out vec3 toEyeView;

uniform vec3 cameraPosition, sunPosition, firePosition, starsPosition;
uniform mat4 model, camera, lightMatrix, projection;
uniform vec4 clipPlane;
uniform bool mirror;

vec4 worldPosition, surfaceView, sunPositionView, firePositionView, starsPositionView;
mat3 normalMatrix;

void main()
{
	normalMatrix = transpose(inverse(mat3(camera * model)));

	worldPosition = model * vec4(position, 1.0);
	if(mirror) { worldPosition.y = -worldPosition.y; };
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projection * camera * worldPosition;

	surfaceView = camera * worldPosition;
	toEyeView = normalize(-vec3(surfaceView));

	sunPositionView = camera * vec4(sunPosition, 1.0);
	toSunView = normalize(vec3(sunPositionView) - vec3(surfaceView));

	firePositionView = camera * vec4(firePosition, 1.0);
	toFireView = normalize(vec3(firePositionView) - vec3(surfaceView));

	starsPositionView = camera * vec4(starsPosition, 1.0);
	toStarsView = normalize(vec3(starsPositionView) - vec3(surfaceView));

	outNormal = normalMatrix * normal;
	outTexCoord = texCoord;
	lightProjectedPos = lightMatrix * worldPosition;
	height = position.y;
}