#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos / 4 + vec2(-1, 1) / 2, 0.0, 1.0);
	TexCoords = aTex;
}