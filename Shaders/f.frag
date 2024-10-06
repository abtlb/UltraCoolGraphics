#version 330
in vec4 vertColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float test;

void main()
{
	vec2 texCoord2 = vec2(texCoord.x, 1 - texCoord.y);
	//fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord2), 0.5);
	fragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
}