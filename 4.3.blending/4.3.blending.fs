#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureID;

void main()
{
    vec4 texColor = texture(textureID, TexCoords);
    //if (texColor.a < 0.1)
    //    discard;
    FragColor = texColor;
}
