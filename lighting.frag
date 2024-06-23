#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}

uniform Material material;

void main()
{
    float ambientStrength = 0.2;
    vec3 ambient = lightColor * ambientStrength;
    
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    float diffuseComponent = max(dot(norm, lightDir), 0);
    vec3 diffuse = diffuseComponent * lightColor;

    float shininess = 128.0;
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), shininess);
    vec3 specular = spec * specularStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}