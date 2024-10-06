#version 330 core

//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
//layout(location = 2) in vec2 aTexCoord;
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

//out vec4 vertColor;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	//gl_Position = vec4(aPos, 1.0);
	//gl_Position = model * vec4(aPos, 1.0);
	//gl_Position = view * model * vec4(aPos, 1.0);
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	//vertColor = vec4(aColor, 1.0);
	texCoord = aTexCoord;
}