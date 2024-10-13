#version 330
in vec3 Normal;
in vec3 Pos;

out vec4 fragColor;

uniform samplerCube cubemap;
uniform vec3 cameraPos;

void main()
{
	vec3 camDir = normalize(Pos - cameraPos);
	float ratio = 1.00 / 1.52;// glass has a refractive index of 1.52
	vec3 refraction = refract(camDir, normalize(Normal), ratio);
	fragColor = texture(cubemap, refraction);
}