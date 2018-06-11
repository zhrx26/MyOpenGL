#version 330 core

out vec4 FragColor;

//in vec3 fColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;

in vec2 TexCoords;

void main()
{
//    FragColor = vec4(fColor, 1.0);

    //FragColor =  vec4(texture(material.texture_diffuse1, TexCoords).rgb, 1.0);

    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
