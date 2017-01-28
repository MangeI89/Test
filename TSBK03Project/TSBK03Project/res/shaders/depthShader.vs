#version 150

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 model, camera, projection, lightMatrix;

void main()
{	
	gl_Position = projection * camera * model * vec4(position, 1.0);
}