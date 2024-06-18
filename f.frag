#version 330
in vec4 vertColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float test;

void main()
{
	fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);
}