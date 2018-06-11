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
    // 线性插值进行颜色混合，第三个参数0.2会返回80%的第一个输入颜色和20%的第二个输入颜色
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), fMixValue);
};