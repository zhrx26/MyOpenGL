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
float fDeltaTime = 0.0f; // 当前帧与上一帧的时间差
float fLastFrame = 0.0f; // 上一帧的时间

Camera pCamera(glm::vec3(0.0f, 1.0f, 5.0f));


float fLastMouseX = WINDOW_WIDTH / 2.0f;
float fLastMouseY = WINDOW_HEIGHT / 2.0f;

float lPointVertices[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
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

	// 启用混合，并设定相应的混合函数
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 启用面剔除
	glEnable(GL_CULL_FACE);

	//Shader pShader("4.9.geometry_shader.vs", "4.9.geometry_shader.fs",  "4.9.geometry_shader.gs");

	//Shader pExplodeModelShader("4.9.geometry_shader.vs", "4.9.geometry_shader.fs", "4.9.explode_model.gs");

	Shader pModelShader("4.9.model.vs", "4.9.model.fs");
	Shader pVisualizingNormalShader("4.9.geometry_shader.vs", "4.9.geometry_shader.fs", "4.9.visualizing_normal.gs");
	
	Model pModel(FileSystem::GetObjectPath("nanosuit/nanosuit.obj"));

	//unsigned int nPointVAO, nPointVBO;
	//// 顶点数组对象
	//glGenVertexArrays(1, &nPointVAO);
	//glBindVertexArray(nPointVAO);
	//// 顶点缓冲对象
	//glGenBuffers(1, &nPointVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, nPointVBO);
	//// 绑定顶点数据和解析格式
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lPointVertices), &lPointVertices, GL_STATIC_DRAW);
	//// 位置属性
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// 颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	// 线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 循环渲染
	while (!glfwWindowShouldClose(pWindow))
	{
		// 检查输入
		ProcessInput(pWindow);

		float fCurrentFrame = glfwGetTime();
		fDeltaTime = fCurrentFrame - fLastFrame;
		fLastFrame = fCurrentFrame;

		glEnable(GL_DEPTH_TEST);
		// 渲染
		//glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// DEPTH_BUFFER_BIT位来清除深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 mProjection;
		// 定义投影矩阵
		mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		// 定义观察矩阵
		glm::mat4 mView;
		mView = pCamera.GetViewMatrix();
		glm::mat4 mModel;

		//pShader.Use();
		//pShader.SetMat4("view", mView);
		//pShader.SetMat4("projection", mProjection);

		//// 绘制4个顶点
		//glBindVertexArray(nPointVAO);
		//mModel = glm::mat4(1.0f);
		//mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, 0.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_POINTS, 0, 4);

		//pExplodeModelShader.Use();
		//pExplodeModelShader.SetFloat("time", glfwGetTime());
		//pExplodeModelShader.SetMat4("view", mView);
		//pExplodeModelShader.SetMat4("projection", mProjection);
		//mModel = glm::mat4(1.0f);
		////mModel = glm::scale(mModel, glm::vec3(0.2, 0.2, 0.2));
		//pExplodeModelShader.SetMat4("model", mModel);
		//pModel.Draw(pExplodeModelShader);
		//glActiveTexture(GL_TEXTURE0);

		pModelShader.Use();
		pModelShader.SetMat4("view", mView);
		pModelShader.SetMat4("projection", mProjection);
		mModel = glm::mat4(1.0f);
		mModel = glm::scale(mModel, glm::vec3(0.2, 0.2, 0.2));
		pModelShader.SetMat4("model", mModel);
		pModel.Draw(pModelShader);
		
		pVisualizingNormalShader.Use();
		pVisualizingNormalShader.SetMat4("view", mView);
		pVisualizingNormalShader.SetMat4("projection", mProjection);
		mModel = glm::mat4(1.0f);
		mModel = glm::scale(mModel, glm::vec3(0.2, 0.2, 0.2));
		pVisualizingNormalShader.SetMat4("model", mModel);
		pModel.Draw(pVisualizingNormalShader);

		glBindVertexArray(0);

		// 检查触发事件
		glfwPollEvents();
		// 交换颜色缓冲
		glfwSwapBuffers(pWindow);
	}

	//glDeleteVertexArrays(1, &nPointVAO);
	//glDeleteBuffers(1, &nPointVBO);

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


unsigned int LoadTexture(char const * sPath)
{
	int nWidth, nHeight, nrChannels;
	unsigned int nTexture;
	glGenTextures(1, &nTexture);
	// 输入图像文件
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

		// 载入图片数据生成一个纹理
		glBindTexture(GL_TEXTURE_2D, nTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, nFormat, nWidth, nHeight, 0, nFormat, GL_UNSIGNED_BYTE, sImage);
		glGenerateMipmap(GL_TEXTURE_2D);
		// 需要用到alpha纹理的时候，将纹理的环绕方式设置为GL_CLAMP_TO_EDGE
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
