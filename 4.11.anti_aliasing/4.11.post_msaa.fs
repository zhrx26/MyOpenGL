#version 330 core

out vec4 FragColor;

uniform sampler2D textureID;

in vec2 TexCoords;

void main()
{
    FragColor = texture(textureID, TexCoords);
}
