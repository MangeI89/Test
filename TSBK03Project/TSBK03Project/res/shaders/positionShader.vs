#version 330 core

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec2 outTexCoord;

void main()
{
	outTexCoord = texCoord;
	gl_Position = vec4(position, 1.0);
}