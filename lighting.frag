#version 330 core

out vec4 FragColor;
uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * material.ambient;
    
    vec3 lightDir = normalize(light.pos - FragPos);
    vec3 norm = normalize(Normal);

    float diffuseComponent = max(dot(norm, lightDir), 0);
    vec3 diffuse = diffuseComponent * material.diffuse * light.diffuse;

    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}