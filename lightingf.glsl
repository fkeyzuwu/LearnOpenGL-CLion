#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D emission;
    float shininess;
};

in vec2 TexCoords;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // light attenuation
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    vec3 diffuseSample = vec3(texture(material.diffuse, TexCoords));
    vec3 ambient = light.ambient * diffuseSample;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseSample;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specSample = vec3(texture(material.specular, TexCoords));
    //vec3 invertedSpec = vec3(1.0) - specSample;
    vec3 specular = light.specular * spec * specSample;

    //vec3 emissionSample = texture(material.emission, TexCoords).rgb;

    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
//    if(specSample == vec3(0))
//    {
//        result = emissionSample;
//    }

    FragColor = vec4(result, 1.0);
}