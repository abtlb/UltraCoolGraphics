#version 330

in vec3 TexDir;

uniform samplerCube cubemap;

out vec4 FragColor;

void main()
{
	FragColor = texture(cubemap, TexDir);
}