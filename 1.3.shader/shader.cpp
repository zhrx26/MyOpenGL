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
	glfwInit();  // ��ʼ��glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // ������ߺ���Ͱ汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ��������
	GLFWwindow * pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MyOpenGL", NULL, NULL);
	if (pWindow == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);
	// ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	// �����ӿڴ�С
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// ���ô��ڴ�С�任ʱ�Ļص�����
	glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);

	unsigned int nVertexShader, nFragmentShader, nShaderProgram;
	int nSuccess;
	char sInfoLog[512];
	// ������ɫ��
	nVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(nVertexShader, 1, &sVertexShaderSource, NULL);
	glCompileShader(nVertexShader);
	glGetShaderiv(nVertexShader, GL_COMPILE_STATUS, &nSuccess);
	if (!nSuccess)
	{
		glGetShaderInfoLog(nVertexShader, 512, NULL, sInfoLog);
		std::cout << "Failed to compile vertex shader" << std::endl;
	}
	// Ƭ����ɫ��
	nFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nFragmentShader, 1, &sFragmentShaderSource, NULL);
	glCompileShader(nFragmentShader);
	glGetShaderiv(nFragmentShader, GL_COMPILE_STATUS, &nSuccess);
	if (!nSuccess)
	{
		glGetShaderInfoLog(nFragmentShader, 512, NULL, sInfoLog);
		std::cout << "Failed to compile fragment shader" << std::endl;
	}
	// ��ɫ������
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

	// �ͷ��Ѿ��󶨵���ɫ��
	glDeleteShader(nVertexShader);
	glDeleteShader(nFragmentShader);

	unsigned int nVAO, nVBO;
	// �����������
	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);
	// ���㻺�����
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);

	// �󶨶������ݺͽ�����ʽ
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// ѭ����Ⱦ
	while (!glfwWindowShouldClose(pWindow))
	{
		// �������
		ProcessInput(pWindow);
		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ��ȡ���е�����
		float fTimeValue = glfwGetTime();
		float fGreenValue = (sin(fTimeValue) / 2.0f) + 0.5f;
		// ��ȡuniform ourColor��λ��ֵ
		int nUniformLocation = glGetUniformLocation(nShaderProgram, "outColor");

		glUseProgram(nShaderProgram);
		//glUniform4f(nUniformLocation, 0.0f, fGreenValue, 0.0f, 1.0f);

		float lValue[4] = {0.0f, fGreenValue, 0.0f, 1.0f};
		// �˴���1��ָunifrom��������Щ�������͵�unifrom���Դ���1
		glUniform4fv(nUniformLocation, 1, lValue);


		glBindVertexArray(nVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ��鴥���¼�
		glfwPollEvents();
		// ������ɫ����
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &nVAO);
	glDeleteBuffers(1, &nVBO);

	// �ر�glfw
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
