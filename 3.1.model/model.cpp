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

glm::vec3 lPointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
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
	Shader pShader("3.1.model.vs", "3.1.model.fs");
	Shader pLampShader("3.1.lamp.vs", "3.1.lamp.fs");

	Model pModel(FileSystem::GetObjectPath("nanosuit/nanosuit.obj"));

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
		mModel = glm::translate(mModel, glm::vec3(0.0f, -2.0f, 0.0f));
		mModel = glm::scale(mModel, glm::vec3(0.2f, 0.2f, 0.2f));

		pShader.SetMat4("view", mView);
		pShader.SetMat4("projection", mProjection);
		pShader.SetMat4("model", mModel);

		pShader.SetFloat("material.shininess", 32.0f);

		// �����
		pShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		pShader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		pShader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// ���Դ 1
		pShader.SetVec3("pointLights[0].position", lPointLightPositions[0]);
		pShader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[0].constant", 1.0f);
		pShader.SetFloat("pointLights[0].linear", 0.09);
		pShader.SetFloat("pointLights[0].quadratic", 0.032);
		// ���Դ 2
		pShader.SetVec3("pointLights[1].position", lPointLightPositions[1]);
		pShader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[1].constant", 1.0f);
		pShader.SetFloat("pointLights[1].linear", 0.09);
		pShader.SetFloat("pointLights[1].quadratic", 0.032);
		// ���Դ 3
		pShader.SetVec3("pointLights[2].position", lPointLightPositions[2]);
		pShader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[2].constant", 1.0f);
		pShader.SetFloat("pointLights[2].linear", 0.09);
		pShader.SetFloat("pointLights[2].quadratic", 0.032);
		// ���Դ 4
		pShader.SetVec3("pointLights[3].position", lPointLightPositions[3]);
		pShader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[3].constant", 1.0f);
		pShader.SetFloat("pointLights[3].linear", 0.09);
		pShader.SetFloat("pointLights[3].quadratic", 0.032);
		// �۹�
		pShader.SetVec3("spotLight.position", pCamera.Position);
		pShader.SetVec3("spotLight.direction", pCamera.Front);
		pShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		pShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		pShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("spotLight.constant", 1.0f);
		pShader.SetFloat("spotLight.linear", 0.09);
		pShader.SetFloat("spotLight.quadratic", 0.032);
		pShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		pShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		pShader.SetVec3("viewPos", pCamera.Position);

		pModel.Draw(pShader);

		// ������ɫ��Դ������ 
		glBindVertexArray(nVAO);
		pLampShader.Use();
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 mLampModel;
			mLampModel = glm::mat4();
			mLampModel = glm::translate(mLampModel, lPointLightPositions[i]);
			mLampModel = glm::scale(mLampModel, glm::vec3(0.2f));
			pLampShader.SetMat4("view", mView);
			pLampShader.SetMat4("projection", mProjection);
			pLampShader.SetMat4("model", mLampModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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
