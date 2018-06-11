#version 330 core

out vec4 FragColor;

//uniform sampler2D textureID;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflection1;
};
uniform Material material;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    //FragColor = texture(textureID, TexCoords);

    vec3 I = normalize(Position - cameraPos);
    // 反射
//    vec3 R = reflect(I, normalize(Normal));

    // 折射，光线/视线从空气进入玻璃
//    float ratio = 1.00 / 1.52;
//    vec3 R = refract(I, normalize(Normal), ratio);
//    FragColor = vec4(texture(skybox, R).rgb, 1.0);

    // 反射贴图
    vec3 reflColor = texture(material.texture_reflection1, TexCoords).rgb;
    vec3 R = reflect(I, normalize(Normal));
    // 将反射出来的效果再乘一次镜面光贴图可以减少噪点
    reflColor = texture(skybox, R).rgb * reflColor * texture(material.texture_specular1, TexCoords).rgb;
    FragColor = vec4(texture(material.texture_diffuse1, TexCoords).rgb + reflColor, 1.0);
}
