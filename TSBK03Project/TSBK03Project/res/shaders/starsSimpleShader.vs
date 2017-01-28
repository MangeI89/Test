#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in mat4 modelMatrix;

uniform sampler2D positionTex;
uniform mat4 camera, projection;
uniform bool mirror;

out vec2 outTexCoord;

void main()
{
	outTexCoord = texCoord;

	vec4 worldPosition = modelMatrix * vec4(position, 0.0, 1.0);
	if(mirror) { 
		worldPosition.y = -worldPosition.y;
	}

	gl_Position = projection * camera * worldPosition;
}  
