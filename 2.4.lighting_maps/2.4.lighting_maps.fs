#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    // ��������
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // ���������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // ��������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ���
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // �������
    vec3 viewDir = normalize(viewPos - FragPos);
    // ���Դ��Ƭ��λ�������ķ�������
    vec3 reflectDir = reflect(-lightDir, norm);
    // �������߷����뷴�䷽��ĵ��, 32�Ǹ߹�ķ����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    
    // �����Է�����ͼ
    result += texture(material.emission, TexCoords).rgb;
    FragColor = vec4(result, 1.0);
};