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

// �����
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

// ���Դ
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

// �۹�
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
    // ���ӷ����
    output += CalcDirLight(dirLight, norm, viewDir);
    // �������е��Դ
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        output += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // ���Ӿ۹�
    output += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    FragColor = vec4(output, 1.0);
};

// ���㶨���
vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ��������
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    // ���������
    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
    // �������
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    return ambient + diffuse + specular;
}

// ������Դ
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance  = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // ��������
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    // ���������
    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
    // �������
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

// ����۹�Դ
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance  = length(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    // ��Եƽ���ľ۹�
    float epsilon = (light.cutOff - light.outerCutOff);
    // clamp�������ѵ�һ������Լ������0.0��1.0֮��
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // ��������
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    // ���������
    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
    // �������
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    diffuse  *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

