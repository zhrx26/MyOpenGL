#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform Material material;

// 定向光
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

// 点光源
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

// 聚光
struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 output;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // 叠加方向光
    output += CalcDirLight(dirLight, norm, viewDir);
    // 叠加所有点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        output += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // 叠加聚光
    output += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    FragColor = vec4(output, 1.0);
};

// 计算定向光
vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 环境光照
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    // 漫放射光照
    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
    // 镜面光照
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    return ambient + diffuse + specular;
}

// 计算点光源
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance  = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 环境光照
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    // 漫放射光照
    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
    // 镜面光照
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

// 计算聚光源
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance  = length(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    // 边缘平滑的聚光
    float epsilon = (light.cutOff - light.outerCutOff);
    // clamp函数，把第一个参数约束在了0.0到1.0之间
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 环境光照
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    // 漫放射光照
    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
    // 镜面光照
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    diffuse  *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

