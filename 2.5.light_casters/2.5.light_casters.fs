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

    // 方向光参数
    float constant;
    float linear;
    float quadratic;
    // 聚光参数
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
    //w分量等于0.0是方向光，w分量等于1.0是平行光, w分量等于0.5是聚光
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

        // 如果lightDir和取反的direction向量点乘得到的cos值小于等于切光角cos值（cos值是从大到小），则使用环境光
        //if(theta <= light.cutOff)
        //{
        //    FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
        //    return;
        //}

        // 边缘平滑的聚光
        float epsilon = (light.cutOff - light.outerCutOff);
        // clamp函数，把第一个参数约束在了0.0到1.0之间
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    }
    // 环境光照
    ambient *= light.ambient * texture(material.diffuse, TexCoords).rgb;
    // 漫放射光照
    vec3 norm = normalize(Normal);
    // 两个向量之间的角度大于90度，点乘的结果就会变成负数
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse *= light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // 镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);
    // 求光源到片段位置向量的反射向量
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算视线方向与反射方向的点乘, 32是高光的反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    specular *= light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0);
};