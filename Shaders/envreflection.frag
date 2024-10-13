#version 330
in vec3 Normal;
in vec3 Pos;

out vec4 fragColor;

uniform samplerCube cubemap;
uniform vec3 cameraPos;

void main()
{
	vec3 camDir = normalize(Pos - cameraPos);
	vec3 reflection = reflect(camDir, normalize(Normal));
	fragColor = texture(cubemap, reflection);
}