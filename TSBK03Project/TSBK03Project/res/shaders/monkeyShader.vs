#version 150

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec2 outTexCoord;

uniform mat4 model, camera, projection;
uniform vec4 clipPlane;
uniform bool mirror;

vec4 worldPosition;

void main()
{
	worldPosition = model * vec4(position, 1.0);
	if(mirror) { worldPosition.y = -worldPosition.y; };
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projection * camera * worldPosition;
	outTexCoord = texCoord;
}