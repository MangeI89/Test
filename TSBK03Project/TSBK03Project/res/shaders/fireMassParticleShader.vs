#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in mat4 modelMatrix;

out vec2 outTexCoord;
out float modInstanceID;

uniform sampler2D velocityTex;
uniform sampler2D positionTex;

uniform mat4 camera, projection;
uniform vec3 cameraForward, offsetPosition;
uniform vec2 screenSize;

vec2 modTexCoord;
out vec2 instID;

void main()
{	
	outTexCoord = texCoord;

	vec3 u = normalize(cross(-cameraForward, vec3(0, 1, 0)));
	vec3 n = cross(u, -cameraForward);

	mat4 modelMatrix = mat4(1.0);
	modelMatrix[0] = vec4(n, 0.0);
	modelMatrix[1] = vec4(u, 0.0);
	modelMatrix[2] = vec4(-cameraForward, 0.0);

	float x = (gl_InstanceID) / screenSize.x;
	modTexCoord.x = mod(x, 1.0);
	modTexCoord.y = (x - modTexCoord.x + 0.5) / screenSize.y;
	modTexCoord.x = mod((gl_InstanceID + 0.5) / screenSize.x, 1.0);
	
	vec3 texPosition = texture2D(positionTex, modTexCoord).rgb - vec3(0.5, 0.5, 0.5);
	vec3 translation = vec3(130*texPosition.x, 50*texPosition.y, 130*texPosition.z);
	vec3 rotatedModelPosition = vec3(modelMatrix * vec4(position.x, position.y, 0.0, 1.0));
	vec4 worldPosition = vec4(rotatedModelPosition + translation + offsetPosition, 1.0);
	gl_Position = projection * camera * worldPosition;
	instID = modTexCoord;
}