#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;

in vec2 TexCoords;

void main()
{
    FragColor =  vec4(texture(material.texture_diffuse1, TexCoords).rgb, 1.0);
}
