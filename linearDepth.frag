#version 330 core

float near = 0.1;
float far = 100.1;

out vec4 FragColor;

void main()
{
    float ndc = gl_FragCoord.z * 2.0 - 1;
    float linearDepth = 1 - ((2.0 * near * far) / (far + near - ndc * (far - near)) / far);

    FragColor = vec4(vec3(linearDepth), 1.0);
}