#version 330 core

out vec4 FragColor;
uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 pos; //not necessary when using directional lights
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //attenuation constants
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.pos - FragPos);
    //vec3 lightDir = normalize(-light.dir);

    vec3 norm = normalize(Normal);

    float diffuseComponent = max(dot(norm, lightDir), 0);
    vec3 diffuse = light.diffuse * diffuseComponent * vec3(texture(material.diffuse, TexCoords));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, TexCoords)) * light.specular;

    float dist = length(light.pos - FragPos);
    float attenuation = 1.0 / ( light.constant + light.linear * dist + light.quadratic * dist * dist);
    vec3 result = (ambient + diffuse + specular) * attenuation;

    FragColor = vec4(result, 1.0);
}