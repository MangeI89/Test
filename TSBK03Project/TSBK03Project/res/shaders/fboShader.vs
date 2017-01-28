#version 150

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec2 outTexCoord;

uniform mat4 model, camera, projection;

void main()
{
	gl_Position = projection * model * vec4(position, 1.0);
	outTexCoord = texCoord;
}