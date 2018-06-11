#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

void FramebufferSizeCallback(GLFWwindow * pWindow, int nWidth, int nHeight);
void ProcessInput(GLFWwindow * pWindow);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

float lVertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

float lVertices2[] = {
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f
};

//float lVertices[] = {
//	0.5f, 0.5f, 0.0f,
//	0.5f, -0.5f, 0.0f,
//	-0.5f,  -0.5f, 0.0f,
//	-0.5f,  0.5f, 0.0f,
//};
//
//unsigned int lIndices[] = {
//	0, 1, 3,
//	1, 2, 3,
//};

const char *sVertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *sFragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

const char *sFragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
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

	// �ڶ�����ɫ������
	unsigned int nFragmentShader2, nShaderProgram2;
	nFragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nFragmentShader2, 1, &sFragmentShaderSource2, NULL);
	glCompileShader(nFragmentShader2);
	glGetShaderiv(nFragmentShader2, GL_COMPILE_STATUS, &nSuccess);
	if (!nSuccess)
	{
		glGetShaderInfoLog(nFragmentShader2, 512, NULL, sInfoLog);
		std::cout << "Failed to compile fragment shader" << std::endl;
	}
	nShaderProgram2 = glCreateProgram();
	glAttachShader(nShaderProgram2, nVertexShader);
	glAttachShader(nShaderProgram2, nFragmentShader2);
	glLinkProgram(nShaderProgram2);
	glGetShaderiv(nShaderProgram2, GL_COMPILE_STATUS, &nSuccess);
	if (!nSuccess)
	{
		glGetProgramInfoLog(nShaderProgram2, 512, NULL, sInfoLog);
		std::cout << "Failed to link shader program" << std::endl;
	}

	// �ͷ��Ѿ��󶨵���ɫ��
	glDeleteShader(nVertexShader);
	glDeleteShader(nFragmentShader);
	glDeleteShader(nFragmentShader2);

	//unsigned int nVAO, nVBO, nEBO;
	//// �����������
	//glGenVertexArrays(1, &nVAO);
	//glBindVertexArray(nVAO);
	//// ���㻺�����
	//glGenBuffers(1, &nVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	////�����������
	////glGenBuffers(1, &nEBO);
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nEBO);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lIndices), lIndices, GL_STATIC_DRAW);

	//// �󶨶������ݺͽ�����ʽ
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//���������������ֲ�ͬ����ɫ������
	unsigned int lVAO[2], lVBO[2];
	glGenVertexArrays(2, lVAO);
	glGenBuffers(2, lVBO);
	glBindVertexArray(lVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, lVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(lVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, lVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices2), lVertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		//glUseProgram(nShaderProgram);
		//glBindVertexArray(nVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// �������������鴴������������
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(nShaderProgram);
		glBindVertexArray(lVAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(nShaderProgram2);
		glBindVertexArray(lVAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ��鴥���¼�
		glfwPollEvents();
		// ������ɫ����
		glfwSwapBuffers(pWindow);
	}

	//glDeleteVertexArrays(1, &nVAO);
	//glDeleteBuffers(1, &nVBO);

	glDeleteVertexArrays(2, lVAO);
	glDeleteBuffers(2, lVBO);

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
