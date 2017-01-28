#version 150

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 model, camera, projection;
uniform vec4 clipPlane;
uniform bool mirror;

vec4 worldPosition;

out vec2 outTexCoord;

void main()
{
	worldPosition = model * vec4(position, 1.0);
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projection * camera * worldPosition;

	outTexCoord = texCoord;
}