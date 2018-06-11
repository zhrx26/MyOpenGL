#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

void FramebufferSizeCallback(GLFWwindow * pWindow, int nWidth, int nHeight);
void ProcessInput(GLFWwindow * pWindow);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

//float lVertices[] = {
//	-0.5f, -0.5f, 0.0f,
//	0.5f, -0.5f, 0.0f,
//	0.0f,  0.5f, 0.0f
//};

float lVertices[] = {
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};

//const char *sVertexShaderSource = "#version 330 core\n"
//	"layout (location = 0) in vec3 aPos;\n"
//	"out vec4 vertexColor;\n"
//	"void main()\n"
//	"{\n"
//	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//	"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
//    "}\0";
//const char *sFragmentShaderSource = "#version 330 core\n"
//    "out vec4 FragColor;\n"
//	"in vec4 vertexColor;\n"
//    "void main()\n"
//    "{\n"
//    "   FragColor = vertexColor;\n"
//    "}\n\0";

//const char *sVertexShaderSource = "#version 330 core\n"
//	"layout (location = 0) in vec3 aPos;\n"
//	"void main()\n"
//	"{\n"
//	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//	"}\0";
//const char *sFragmentShaderSource = "#version 330 core\n"
//	"out vec4 FragColor;\n"
//	"uniform vec4 outColor;\n"
//	"void main()\n"
//	"{\n"
//	"   FragColor = outColor;\n"
//	"}\n\0";

const char *sVertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 outColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"	outColor = aColor;\n"
	"}\0";
const char *sFragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 outColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(outColor, 1.0);\n"
	"}\n\0";


int main()
{
	glfwInit();  // 初始化glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 设置最高和最低版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 创建窗口
	GLFWwindow * pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MyOpenGL", NULL, NULL);
	if (pWindow == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);
	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	// 设置视口大小
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// 设置窗口大小变换时的回调函数
	glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);

	unsigned int nVertexShader, nFragmentShader, nShaderProgram;
	int nSuccess;
	char sInfoLog[512];
	// 顶点着色器
	nVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(nVertexShader, 1, &sVertexShaderSource, NULL);
	glCompileShader(nVertexShader);
	glGetShaderiv(nVertexShader, GL_COMPILE_STATUS, &nSuccess);
	if (!nSuccess)
	{
		glGetShaderInfoLog(nVertexShader, 512, NULL, sInfoLog);
		std::cout << "Failed to compile vertex shader" << std::endl;
	}
	// 片段着色器
	nFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nFragmentShader, 1, &sFragmentShaderSource, NULL);
	glCompileShader(nFragmentShader);
	glGetShaderiv(nFragmentShader, GL_COMPILE_STATUS, &nSuccess);
	if (!nSuccess)
	{
		glGetShaderInfoLog(nFragmentShader, 512, NULL, sInfoLog);
		std::cout << "Failed to compile fragment shader" << std::endl;
	}
	// 着色器程序
	nShaderProgram = glCreateProgram();
	glAttachShader(nShaderProgram, nVertexShader);
	glAttachShader(nShaderProgram, nFragmentShader);
	glLinkProgram(nShaderProgram);
	glGetShaderiv(nShaderProgram, GL_COMPILE_STATUS, &nSuccess);
	if (!nSuccess)
	{
		glGetProgramInfoLog(nShaderProgram, 512, NULL, sInfoLog);
		std::cout << "Failed to link shader program" << std::endl;
	}

	// 释放已经绑定的着色器
	glDeleteShader(nVertexShader);
	glDeleteShader(nFragmentShader);

	unsigned int nVAO, nVBO;
	// 顶点数组对象
	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);
	// 顶点缓冲对象
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);

	// 绑定顶点数据和解析格式
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// 循环渲染
	while (!glfwWindowShouldClose(pWindow))
	{
		// 检查输入
		ProcessInput(pWindow);
		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 获取运行的秒数
		float fTimeValue = glfwGetTime();
		float fGreenValue = (sin(fTimeValue) / 2.0f) + 0.5f;
		// 获取uniform ourColor的位置值
		int nUniformLocation = glGetUniformLocation(nShaderProgram, "outColor");

		glUseProgram(nShaderProgram);
		//glUniform4f(nUniformLocation, 0.0f, fGreenValue, 0.0f, 1.0f);

		float lValue[4] = {0.0f, fGreenValue, 0.0f, 1.0f};
		// 此处的1是指unifrom个数，有些数组类型的unifrom可以大于1
		glUniform4fv(nUniformLocation, 1, lValue);


		glBindVertexArray(nVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 检查触发事件
		glfwPollEvents();
		// 交换颜色缓冲
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &nVAO);
	glDeleteBuffers(1, &nVBO);

	// 关闭glfw
	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow * pWindow, int nWidth, int nHeight)
{
	glViewport(0, 0, nWidth, nHeight);
}

void ProcessInput(GLFWwindow * pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, true);
}
