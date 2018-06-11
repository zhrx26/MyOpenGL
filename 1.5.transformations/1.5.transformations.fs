#version 330 core

out vec4 FragColor;
in vec3 outColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float fMixValue;

void main()
{
    //FragColor = texture(texture1, TexCoord) * vec4(outColor, 1.0);
    // ���Բ�ֵ������ɫ��ϣ�����������0.2�᷵��80%�ĵ�һ��������ɫ��20%�ĵڶ���������ɫ
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), fMixValue);
};