#version 330 core

layout (points) in;
//layout (line_strip, max_vertices = 2) out;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_line(vec4 position)
{
	// 绘制线段
	gl_Position = position + vec4(-0.1, 0.0, 0.0, 0.0);
	EmitVertex();

	gl_Position = position + vec4(0.1, 0.0, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}


void build_house(vec4 position)
{
	fColor = gs_in[0].color;
	// 绘制三角形
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);  // 左下
	EmitVertex();

	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);  // 右下
	EmitVertex();

	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);  // 左上
	EmitVertex();

	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);  // 右上
	EmitVertex();

	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);  // 顶部
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	//build_line(gl_in[0].gl_Position);

	build_house(gl_in[0].gl_Position);
}