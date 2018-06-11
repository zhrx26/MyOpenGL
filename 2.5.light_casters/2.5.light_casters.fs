#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light {
    //vec3 position;
    vec4 lightVector;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // ��������
    float constant;
    float linear;
    float quadratic;
    // �۹����
    vec3 direction;
    float cutOff;
    float outerCutOff;
};
uniform Light light;

void main()
{
    vec3 lightDir;
    vec3 ambient = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0);
    vec3 specular = vec3(1.0, 1.0, 1.0);
    //w��������0.0�Ƿ���⣬w��������1.0��ƽ�й�, w��������0.5�Ǿ۹�
    if(light.lightVector.w == 0.0)
    {
        lightDir = normalize(light.lightVector.rgb - FragPos);
        float distance  = length(light.lightVector.rgb - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }
    else if(light.lightVector.w == 1.0)
        lightDir = normalize(-light.lightVector.rgb);
    else if(light.lightVector.w == 0.5)
    {
        lightDir = normalize(light.lightVector.rgb - FragPos);
        float theta = dot(lightDir, normalize(-light.direction));

        // ���lightDir��ȡ����direction������˵õ���cosֵС�ڵ����й��cosֵ��cosֵ�ǴӴ�С������ʹ�û�����
        //if(theta <= light.cutOff)
        //{
        //    FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
        //    return;
        //}

        // ��Եƽ���ľ۹�
        float epsilon = (light.cutOff - light.outerCutOff);
        // clamp�������ѵ�һ������Լ������0.0��1.0֮��
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    }
    // ��������
    ambient *= light.ambient * texture(material.diffuse, TexCoords).rgb;
    // ���������
    vec3 norm = normalize(Normal);
    // ��������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ���
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse *= light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // �������
    vec3 viewDir = normalize(viewPos - FragPos);
    // ���Դ��Ƭ��λ�������ķ�������
    vec3 reflectDir = reflect(-lightDir, norm);
    // �������߷����뷴�䷽��ĵ��, 32�Ǹ߹�ķ����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    specular *= light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0);
};