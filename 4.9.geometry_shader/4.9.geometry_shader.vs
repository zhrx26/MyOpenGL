#version 330 core

//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec3 aColor;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out VS_OUT{
//	vec3 color;
//} vs_out;

out VS_OUT{
	vec3 normal;
	vec2 texCoords;
} vs_out;


void main()
{
	//gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
	//vs_out.color = aColor;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.texCoords = aTexCoords;
	// 将法向量变换到剪裁空间坐标
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));
}