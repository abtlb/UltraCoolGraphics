#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 Normal;
out vec3 FragPos;//it is cheaper to compute this here
void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);//position on screen
    FragPos = vec3(model * vec4(aPos, 1.0));//position in world space
    Normal = mat3(transpose(inverse(model))) * aNormal;
}