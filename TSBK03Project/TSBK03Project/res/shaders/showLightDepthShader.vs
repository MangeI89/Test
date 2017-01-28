#version 150

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec2 outTexCoord;

void main()
{
	gl_Position = vec4(position / 4.0 + vec3(3.0/4.0, 3.0/4.0, 0.0), 1.0);
	outTexCoord = texCoord;
}