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
    // 环境光照
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // 漫放射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // 两个向量之间的角度大于90度，点乘的结果就会变成负数
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // 镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);
    // 求光源到片段位置向量的反射向量
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算视线方向与反射方向的点乘, 32是高光的反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    
    // 叠加自发光贴图
    result += texture(material.emission, TexCoords).rgb;
    FragColor = vec4(result, 1.0);
};