#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Pos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = proj * view * vec4(Pos, 1.0f);
}