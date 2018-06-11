#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 ambient = light.ambient *  material.ambient;
    // ���������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // ��������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ���
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff *  material.diffuse);
    // �������
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    // ���Դ��Ƭ��λ�������ķ�������
    vec3 reflectDir = reflect(-lightDir, norm);
    // �������߷����뷴�䷽��ĵ��, 32�Ǹ߹�ķ����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * (material.specular * spec);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
};