#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>

#include<tools/filesystem.h>
#include<tools/shader.h>
#include<tools/camera.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>

void FramebufferSizeCallback(GLFWwindow * pWindow, int nWidth, int nHeight);
void MouseMoveCallback(GLFWwindow* pWindow, double xPos, double yPos);
void MouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset);
void ProcessInput(GLFWwindow * pWindow);
unsigned int LoadTexture(char const * sPath);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

bool bFirstMouse = true;
float fDeltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float fLastFrame = 0.0f; // ��һ֡��ʱ��

Camera pCamera(glm::vec3(0.0f, 1.0f, 5.0f));


float fLastMouseX = WINDOW_WIDTH / 2.0f;
float fLastMouseY = WINDOW_HEIGHT / 2.0f;

float lVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

	// ������װ����Shader����
	Shader pShader("2.4.lighting_maps.vs", "2.4.lighting_maps.fs");
	Shader pLampShader("2.4.lamp.vs", "2.4.lamp.fs");

	unsigned int nVAO, nVBO, nEBO;
	// �����������
	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);
	// ���㻺�����
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);

	// �󶨶������ݺͽ�����ʽ
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ����������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ������������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int nDiffuseMap = LoadTexture(FileSystem::GetTexturePath("container2.png").c_str());
	unsigned int nSpecularMap = LoadTexture(FileSystem::GetTexturePath("container2.png").c_str());
	unsigned int nEmissionMap = LoadTexture(FileSystem::GetTexturePath("matrix.jpg").c_str());

	// ��ɫ�ľ������ͼ
	//unsigned int nSpecularMap = LoadTexture(FileSystem::GetTexturePath("lighting_maps_specular_color.png").c_str());
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ѭ����Ⱦ
	while (!glfwWindowShouldClose(pWindow))
	{
		// �������
		ProcessInput(pWindow);


		float fCurrentFrame = glfwGetTime();
		fDeltaTime = fCurrentFrame - fLastFrame;
		fLastFrame = fCurrentFrame;

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// DEPTH_BUFFER_BITλ�������Ȼ���
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 vLightPos(sin((float)glfwGetTime()), 0.8f, 2.0f);

		pShader.Use();
		glm::mat4 mProjection;
		// ����ͶӰ����
		mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		// ����۲����
		glm::mat4 mView;
		mView = pCamera.GetViewMatrix();
		glm::mat4 mModel;
		mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, 0.0f));

		pShader.SetMat4("view", mView);
		pShader.SetMat4("projection", mProjection);
		pShader.SetMat4("model", mModel);


		pShader.SetInt("material.diffuse", 0);
		pShader.SetInt("material.specular", 1);
		pShader.SetInt("material.emission", 2);
		pShader.SetFloat("material.shininess", 32.0f);
		// ��Դ�����⡢������;��������ϲ�ͬ��ǿ��
		pShader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//pShader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		pShader.SetVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
		pShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//pShader.SetVec3("light.ambient", 1.0f, 1.0f, 1.0f);
		//pShader.SetVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
		//pShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		pShader.SetVec3("light.position", vLightPos);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nDiffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, nSpecularMap);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, nEmissionMap);

		pShader.SetVec3("viewPos", pCamera.Position);

		glBindVertexArray(nVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ������ɫ��Դ������ 
		pLampShader.Use();
		mModel = glm::mat4();
		mModel = glm::translate(mModel, vLightPos);
		mModel = glm::scale(mModel, glm::vec3(0.2f));
		pLampShader.SetMat4("view", mView);
		pLampShader.SetMat4("projection", mProjection);
		pLampShader.SetMat4("model", mModel);
		glBindVertexArray(nVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ��鴥���¼�
		glfwPollEvents();
		// ������ɫ����
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &nVAO);
	glDeleteBuffers(1, &nVBO);
	glDeleteBuffers(1, &nEBO);

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, nFormat, GL_UNSIGNED_BYTE, sImage);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Failed to load texture" << sPath << std::endl;
	stbi_image_free(sImage);
	return nTexture;
}
