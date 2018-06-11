#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0);
	//gl_Position = pos;
	// 将z分量设置为w，执行透视除法后z分量就变为w/w=1.0
	gl_Position = pos.xyww;
}