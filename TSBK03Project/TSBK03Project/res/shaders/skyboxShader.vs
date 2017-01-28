#version 150

attribute vec3 position;
attribute vec2 texCoord;

out vec3 skyboxTexCoord;

uniform mat4 model, camera, projection;
uniform bool mirror;
uniform vec4 clipPlane;

vec4 worldPosition;

void main()
{
	worldPosition = vec4(position, 1.0);
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projection * camera * worldPosition;

	if(mirror) {
		skyboxTexCoord = vec3(position.x, -position.y, position.z);
	} else {
		skyboxTexCoord = position;
	}
	
}