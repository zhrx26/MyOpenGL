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
    // ����
//    vec3 R = reflect(I, normalize(Normal));

    // ���䣬����/���ߴӿ������벣��
//    float ratio = 1.00 / 1.52;
//    vec3 R = refract(I, normalize(Normal), ratio);
//    FragColor = vec4(texture(skybox, R).rgb, 1.0);

    // ������ͼ
    vec3 reflColor = texture(material.texture_reflection1, TexCoords).rgb;
    vec3 R = reflect(I, normalize(Normal));
    // �����������Ч���ٳ�һ�ξ������ͼ���Լ������
    reflColor = texture(skybox, R).rgb * reflColor * texture(material.texture_specular1, TexCoords).rgb;
    FragColor = vec4(texture(material.texture_diffuse1, TexCoords).rgb + reflColor, 1.0);
}
