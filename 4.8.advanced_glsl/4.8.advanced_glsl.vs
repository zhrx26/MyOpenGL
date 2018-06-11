#version 330 core

layout (location = 0) in vec3 aPos;

// OpenGL4.2起可以使用布局标识符来显式绑定
//layout (std140, binding=0) uniform Matrices

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// 设置顶点的大小
	//gl_PointSize = gl_Position.z * 10;
}