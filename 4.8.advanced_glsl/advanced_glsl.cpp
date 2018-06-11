#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>

#include<tools/filesystem.h>
#include<tools/shader.h>
#include<tools/camera.h>
#include<tools/model.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<vector>
#include<map>
#include<iostream>
using namespace std;

void FramebufferSizeCallback(GLFWwindow * pWindow, int nWidth, int nHeight);
void MouseMoveCallback(GLFWwindow* pWindow, double xPos, double yPos);
void MouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset);
void ProcessInput(GLFWwindow * pWindow);
unsigned int LoadTexture(char const * sPath);
unsigned int LoadCubeMaps(vector<std::string> vFace);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

bool bFirstMouse = true;
float fDeltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float fLastFrame = 0.0f; // ��һ֡��ʱ��

Camera pCamera(glm::vec3(0.0f, 1.0f, 5.0f));


float fLastMouseX = WINDOW_WIDTH / 2.0f;
float fLastMouseY = WINDOW_HEIGHT / 2.0f;


float lPointVertices[] = {
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
};

float lCubeVertices[] = {
	// ����
	-0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	// ����
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	// ����
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	// ����
	0.5f,  0.5f,  0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f, 

	// ����
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	// ����
	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
};

glm::vec3 vCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 vCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 vCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
	// ��׽���
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// ���ô��ڴ�С�任ʱ�Ļص�����
	glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);
	// ����ƶ��任ʱ�Ļص�����
	glfwSetCursorPosCallback(pWindow, MouseMoveCallback);
	// �����ֱ任ʱ�Ļص�����
	glfwSetScrollCallback(pWindow, MouseScrollCallback);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// ���û�ϣ����趨��Ӧ�Ļ�Ϻ���
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// �������޳�
	//glEnable(GL_CULL_FACE);

	// �����޸ĵ��С�Ĺ���
	//glEnable(GL_PROGRAM_POINT_SIZE);

	//Shader pShader("4.8.advanced_glsl.vs", "4.8.advanced_glsl.fs");

	Shader pRedShader("4.8.advanced_glsl.vs", "4.8.red.fs");
	Shader pGreenShader("4.8.advanced_glsl.vs", "4.8.green.fs");
	Shader pBlueShader("4.8.advanced_glsl.vs", "4.8.blue.fs");
	Shader pYellowShader("4.8.advanced_glsl.vs", "4.8.yellow.fs");

	unsigned int nCubeVAO, nCubeVBO;
	// �����������
	glGenVertexArrays(1, &nCubeVAO);
	glBindVertexArray(nCubeVAO);
	// ���㻺�����
	glGenBuffers(1, &nCubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nCubeVBO);
	// �󶨶������ݺͽ�����ʽ
	glBufferData(GL_ARRAY_BUFFER, sizeof(lCubeVertices), &lCubeVertices, GL_STATIC_DRAW);
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//unsigned int nPointVAO, nPointVBO;
	//// �����������
	//glGenVertexArrays(1, &nPointVAO);
	//glBindVertexArray(nPointVAO);
	//// ���㻺�����
	//glGenBuffers(1, &nPointVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, nPointVBO);
	//// �󶨶������ݺͽ�����ʽ
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lPointVertices), &lPointVertices, GL_STATIC_DRAW);
	//// λ������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	// ��ȡ�Ѷ���Uniform���λ��ֵ����
	unsigned int nUniformBlockIndexRed = glGetUniformBlockIndex(pRedShader.m_nID, "Matrices");
	unsigned int nUniformBlockIndexGreen = glGetUniformBlockIndex(pGreenShader.m_nID, "Matrices");
	unsigned int nUniformBlockIndexBlue = glGetUniformBlockIndex(pBlueShader.m_nID, "Matrices");
	unsigned int nUniformBlockIndexYellow = glGetUniformBlockIndex(pYellowShader.m_nID, "Matrices");

	// ����ȡ����Uniform��󶨵��ض��İ󶨵�
	glUniformBlockBinding(pRedShader.m_nID, nUniformBlockIndexRed, 0);
	glUniformBlockBinding(pGreenShader.m_nID, nUniformBlockIndexGreen, 0);
	glUniformBlockBinding(pBlueShader.m_nID, nUniformBlockIndexBlue, 0);
	glUniformBlockBinding(pYellowShader.m_nID, nUniformBlockIndexYellow, 0);

	// ����Uniform�������
	unsigned int nUboMatrices;
	glGenBuffers(1, &nUboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, nUboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// ���ְ�Uniform�������ĺ�����glBindBufferRange���������ö����ͬ��Uniform��󶨵�ͬһ��Uniform���������
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, nUboMatrices);
	// ��Uniform�������󶨵��󶨵�0��
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, nUboMatrices, 0, 2 * (sizeof(glm::mat4)));

	// ͶӰ����
	glm::mat4 mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, nUboMatrices);
	// ʹ��glBufferSubData�����������ݸ��µ�Uniform������
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mProjection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// ѭ����Ⱦ
	while (!glfwWindowShouldClose(pWindow))
	{
		// �������
		ProcessInput(pWindow);

		float fCurrentFrame = glfwGetTime();
		fDeltaTime = fCurrentFrame - fLastFrame;
		fLastFrame = fCurrentFrame;

		glEnable(GL_DEPTH_TEST);
		// ��Ⱦ
		//glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// DEPTH_BUFFER_BITλ�������Ȼ���
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 mProjection;
		//// ����ͶӰ����
		//mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		//// ����۲����
		//glm::mat4 mView;
		//mView = pCamera.GetViewMatrix();
		//glm::mat4 mModel;

		//pShader.Use();
		//pShader.SetMat4("view", mView);
		//pShader.SetMat4("projection", mProjection);
		//pShader.SetVec3("cameraPos", pCamera.Position);
		//glBindVertexArray(nCubeVAO);
		//mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, -4.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//mModel = glm::mat4(1.0f);
		//mModel = glm::translate(mModel, glm::vec3(-2.0f, 0.0f, 0.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//// ����4������
		//glBindVertexArray(nPointVAO);
		//mModel = glm::mat4(1.0f);
		//mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, 0.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_POINTS, 0, 4);

		// �۲����
		glm::mat4 mView = pCamera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, nUboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mView));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glm::mat4 mModel;
		glBindVertexArray(nCubeVAO);

		pRedShader.Use();
		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(-1.0f, 1.0f, 0.0f));
		pRedShader.SetMat4("model", mModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		pGreenShader.Use();
		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(1.0f, 1.0f, 0.0f));
		pGreenShader.SetMat4("model", mModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		pBlueShader.Use();
		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(-1.0f, -1.0f, 0.0f));
		pBlueShader.SetMat4("model", mModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		pYellowShader.Use();
		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(1.0f, -1.0f, 0.0f));
		pYellowShader.SetMat4("model", mModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// ��鴥���¼�
		glfwPollEvents();
		// ������ɫ����
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &nCubeVAO);
	//glDeleteVertexArrays(1, &nPointVAO);
	glDeleteBuffers(1, &nCubeVBO);
	//glDeleteBuffers(1, &nPointVBO);

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
	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
		pCamera.ProcessKeyboard(FORWARD, fDeltaTime);
	if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
		pCamera.ProcessKeyboard(BACKWARD, fDeltaTime);
	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
		pCamera.ProcessKeyboard(LEFT, fDeltaTime);
	if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
		pCamera.ProcessKeyboard(RIGHT, fDeltaTime);
}


void MouseMoveCallback(GLFWwindow* pWindow, double xPos, double yPos)
{
	if (bFirstMouse)
	{
		fLastMouseX = xPos;
		fLastMouseY = yPos;
		bFirstMouse = false;
	}

	float fXOffset = xPos - fLastMouseX;
	float fYOffset = fLastMouseY - yPos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
	fLastMouseX = xPos;
	fLastMouseY = yPos;

	pCamera.ProcessMouseMovement(fXOffset, fYOffset);
}


void MouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
	pCamera.ProcessMouseScroll(yOffset);
}


unsigned int LoadTexture(char const * sPath)
{
	int nWidth, nHeight, nrChannels;
	unsigned int nTexture;
	glGenTextures(1, &nTexture);
	// ����ͼ���ļ�
	unsigned char *sImage = stbi_load(sPath, &nWidth, &nHeight, &nrChannels, 0);
	if (sImage)
	{
		GLenum nFormat;
		if (nrChannels == 1)
			nFormat = GL_RED;
		else if (nrChannels == 3)
			nFormat = GL_RGB;
		else if (nrChannels == 4)
			nFormat = GL_RGBA;

		// ����ͼƬ��������һ������
		glBindTexture(GL_TEXTURE_2D, nTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, nFormat, nWidth, nHeight, 0, nFormat, GL_UNSIGNED_BYTE, sImage);
		glGenerateMipmap(GL_TEXTURE_2D);
		// ��Ҫ�õ�alpha�����ʱ�򣬽�����Ļ��Ʒ�ʽ����ΪGL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, nFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, nFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Failed to load texture" << sPath << std::endl;
	stbi_image_free(sImage);
	return nTexture;
}
