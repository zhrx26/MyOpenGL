#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

//uniform vec2 offsets[100];

out vec3 fColor;


void main()
{
	fColor = aColor;
	//vec2 offset = offsets[gl_InstanceID];
    //gl_Position = vec4(aPos + offset, 0.0, 1.0);
	
    //gl_Position = vec4(aPos * + aOffset, 0.0, 1.0);

	// 随着实例化数组的序号越来越大
    gl_Position = vec4(aPos * (gl_InstanceID / 100.0) + aOffset, 0.0, 1.0);
}