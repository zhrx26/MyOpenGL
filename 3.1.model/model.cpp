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
float fDeltaTime = 0.0f; // 当前帧与上一帧的时间差
float fLastFrame = 0.0f; // 上一帧的时间

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
	// 捕捉光标
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 设置窗口大小变换时的回调函数
	glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);
	// 鼠标移动变换时的回调函数
	glfwSetCursorPosCallback(pWindow, MouseMoveCallback);
	// 鼠标滚轮变换时的回调函数
	glfwSetScrollCallback(pWindow, MouseScrollCallback);

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);

	// 创建封装过的Shader对象
	Shader pShader("3.1.model.vs", "3.1.model.fs");
	Shader pLampShader("3.1.lamp.vs", "3.1.lamp.fs");

	Model pModel(FileSystem::GetObjectPath("nanosuit/nanosuit.obj"));

	unsigned int nVAO, nVBO, nEBO;
	// 顶点数组对象
	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);
	// 顶点缓冲对象
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);

	// 绑定顶点数据和解析格式
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 法向量属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 纹理坐标属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 循环渲染
	while (!glfwWindowShouldClose(pWindow))
	{
		// 检查输入
		ProcessInput(pWindow);


		float fCurrentFrame = glfwGetTime();
		fDeltaTime = fCurrentFrame - fLastFrame;
		fLastFrame = fCurrentFrame;

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// DEPTH_BUFFER_BIT位来清除深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 vLightPos(sin((float)glfwGetTime()), 0.8f, 2.0f);

		pShader.Use();
		glm::mat4 mProjection;
		// 定义投影矩阵
		mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		// 定义观察矩阵
		glm::mat4 mView;
		mView = pCamera.GetViewMatrix();
		glm::mat4 mModel;
		mModel = glm::translate(mModel, glm::vec3(0.0f, -2.0f, 0.0f));
		mModel = glm::scale(mModel, glm::vec3(0.2f, 0.2f, 0.2f));

		pShader.SetMat4("view", mView);
		pShader.SetMat4("projection", mProjection);
		pShader.SetMat4("model", mModel);

		pShader.SetFloat("material.shininess", 32.0f);

		// 定向光
		pShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		pShader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		pShader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// 点光源 1
		pShader.SetVec3("pointLights[0].position", lPointLightPositions[0]);
		pShader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[0].constant", 1.0f);
		pShader.SetFloat("pointLights[0].linear", 0.09);
		pShader.SetFloat("pointLights[0].quadratic", 0.032);
		// 点光源 2
		pShader.SetVec3("pointLights[1].position", lPointLightPositions[1]);
		pShader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[1].constant", 1.0f);
		pShader.SetFloat("pointLights[1].linear", 0.09);
		pShader.SetFloat("pointLights[1].quadratic", 0.032);
		// 点光源 3
		pShader.SetVec3("pointLights[2].position", lPointLightPositions[2]);
		pShader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[2].constant", 1.0f);
		pShader.SetFloat("pointLights[2].linear", 0.09);
		pShader.SetFloat("pointLights[2].quadratic", 0.032);
		// 点光源 4
		pShader.SetVec3("pointLights[3].position", lPointLightPositions[3]);
		pShader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		pShader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		pShader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		pShader.SetFloat("pointLights[3].constant", 1.0f);
		pShader.SetFloat("pointLights[3].linear", 0.09);
		pShader.SetFloat("pointLights[3].quadratic", 0.032);
		// 聚光
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

		// 创建白色光源立方体 
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

		// 检查触发事件
		glfwPollEvents();
		// 交换颜色缓冲
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &nVAO);
	glDeleteBuffers(1, &nVBO);
	glDeleteBuffers(1, &nEBO);

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
	float fYOffset = fLastMouseY - yPos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	fLastMouseX = xPos;
	fLastMouseY = yPos;

	pCamera.ProcessMouseMovement(fXOffset, fYOffset);
}


void MouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
	pCamera.ProcessMouseScroll(yOffset);
}
