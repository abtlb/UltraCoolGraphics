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
uniform Material material;//we only need one material per fragment, right?

//struct Light
//{
//    vec3 pos; //not necessary when using directional lights
//    vec3 dir;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//
//    //attenuation constants
//    float constant;
//    float linear;
//    float quadratic;
//
//    float innerCutOff;//cos angle
//    float outerCutOff;//cos angle
//};
//uniform Light light;

struct DirLight
{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight
{
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //attenuation constants
    float constant;
    float linear;
    float quadratic;
};
#define POINT_LIGHTS_NUM 4
uniform PointLight pointLights[POINT_LIGHTS_NUM];

struct Spotlight
{
    vec3 pos;
    vec3 dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //attenuation constants
    float constant;
    float linear;
    float quadratic;

    float innerCutOff;//cos angle
    float outerCutOff;//cos angle
};
uniform Spotlight flashlight;

vec3 GetDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.dir);//lightDir pointing to the light source

    float diffuseComponent = max(dot(norm, lightDir), 0);
    vec3 diffuse = light.diffuse * diffuseComponent * texture(material.diffuse, TexCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, TexCoords)) * light.specular;

    vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;


    return ambient + specular + diffuse;
}

vec3 GetPointLight(PointLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - FragPos);

    float diffuseComponent = max(dot(norm, lightDir), 0);
    vec3 diffuse = light.diffuse * diffuseComponent * texture(material.diffuse, TexCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, TexCoords)) * light.specular;

    vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;

    float dist = length(light.pos - FragPos);
    float attenuation = 1.0 / ( light.constant + light.linear * dist + light.quadratic * dist * dist);
    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    return ambient + specular + diffuse;
}

vec3 GetSpotlight(Spotlight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - FragPos);

    float diffuseComponent = max(dot(norm, lightDir), 0);
    vec3 diffuse = light.diffuse * diffuseComponent * texture(material.diffuse, TexCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, TexCoords)) * light.specular;

    vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;

    float dist = length(light.pos - FragPos);
    float attenuation = 1.0 / ( light.constant + light.linear * dist + light.quadratic * dist * dist);
    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    float theta = dot(lightDir, normalize(-light.dir));//cos
    float epsilon = light.innerCutOff - light.outerCutOff;//remember, we're subtracting cosines
    float intensity =  clamp((theta - light.outerCutOff) / epsilon, 0, 1);// I = (theta - outer) / epsilon

    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

//void main()
//{
//    vec3 lightDir = normalize(light.pos - FragPos);//point light/spotlight 
//    //vec3 lightDir = normalize(-light.dir);//directional
//
//    vec3 norm = normalize(Normal);
//
//    float diffuseComponent = max(dot(norm, lightDir), 0);
//    vec3 diffuse = light.diffuse * diffuseComponent * vec3(texture(material.diffuse, TexCoords));
//
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//
//    vec3 viewDir = normalize(cameraPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
//    vec3 specular = spec * vec3(texture(material.specular, TexCoords)) * light.specular;
//
//    float dist = length(light.pos - FragPos);
//    float attenuation = 1.0 / ( light.constant + light.linear * dist + light.quadratic * dist * dist);
//    ambient *= attenuation;
//    specular *= attenuation;
//    diffuse *= attenuation;
//
//    float theta = dot(lightDir, normalize(-light.dir));//cos
//    float epsilon = light.innerCutOff - light.outerCutOff;//remember, we're subtracting cosines
//    float intensity =  clamp((theta - light.outerCutOff) / epsilon, 0, 1);// I = (theta - outer) / epsilon
//    diffuse *= intensity;
//    specular *= intensity;
//
//    vec3 result = ambient + diffuse + specular;
//
//    FragColor = vec4(result, 1.0);
//}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 result = vec3(0.0);
    result += GetDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < POINT_LIGHTS_NUM; i++)
    {
        result += GetPointLight(pointLights[i], norm, viewDir);
    }
    result += GetSpotlight(flashlight, norm, viewDir);

    FragColor = vec4(result, 1.0);
}