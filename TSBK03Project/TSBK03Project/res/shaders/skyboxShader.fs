#version 150

in vec3 skyboxTexCoord;

uniform samplerCube daySkybox, nightSkybox;
uniform float blendFactor;

void main()
{
	gl_FragColor = mix(texture(daySkybox, skyboxTexCoord), texture(nightSkybox, skyboxTexCoord), blendFactor);
}