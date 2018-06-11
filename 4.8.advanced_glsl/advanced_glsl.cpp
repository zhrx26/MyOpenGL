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
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
};

float lCubeVertices[] = {
	// 背面
	-0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	// 正面
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	// 左面
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	// 右面
	0.5f,  0.5f,  0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f, 

	// 下面
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	// 上面
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
	//glEnable(GL_CULL_FACE);

	// 启用修改点大小的功能
	//glEnable(GL_PROGRAM_POINT_SIZE);

	//Shader pShader("4.8.advanced_glsl.vs", "4.8.advanced_glsl.fs");

	Shader pRedShader("4.8.advanced_glsl.vs", "4.8.red.fs");
	Shader pGreenShader("4.8.advanced_glsl.vs", "4.8.green.fs");
	Shader pBlueShader("4.8.advanced_glsl.vs", "4.8.blue.fs");
	Shader pYellowShader("4.8.advanced_glsl.vs", "4.8.yellow.fs");

	unsigned int nCubeVAO, nCubeVBO;
	// 顶点数组对象
	glGenVertexArrays(1, &nCubeVAO);
	glBindVertexArray(nCubeVAO);
	// 顶点缓冲对象
	glGenBuffers(1, &nCubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nCubeVBO);
	// 绑定顶点数据和解析格式
	glBufferData(GL_ARRAY_BUFFER, sizeof(lCubeVertices), &lCubeVertices, GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	// 获取已定义Uniform块的位置值索引
	unsigned int nUniformBlockIndexRed = glGetUniformBlockIndex(pRedShader.m_nID, "Matrices");
	unsigned int nUniformBlockIndexGreen = glGetUniformBlockIndex(pGreenShader.m_nID, "Matrices");
	unsigned int nUniformBlockIndexBlue = glGetUniformBlockIndex(pBlueShader.m_nID, "Matrices");
	unsigned int nUniformBlockIndexYellow = glGetUniformBlockIndex(pYellowShader.m_nID, "Matrices");

	// 将获取到的Uniform块绑定到特定的绑定点
	glUniformBlockBinding(pRedShader.m_nID, nUniformBlockIndexRed, 0);
	glUniformBlockBinding(pGreenShader.m_nID, nUniformBlockIndexGreen, 0);
	glUniformBlockBinding(pBlueShader.m_nID, nUniformBlockIndexBlue, 0);
	glUniformBlockBinding(pYellowShader.m_nID, nUniformBlockIndexYellow, 0);

	// 创建Uniform缓冲对象
	unsigned int nUboMatrices;
	glGenBuffers(1, &nUboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, nUboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// 两种绑定Uniform缓冲对象的函数，glBindBufferRange函数可以让多个不同的Uniform块绑定到同一个Uniform缓冲对象上
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, nUboMatrices);
	// 将Uniform缓冲对象绑定到绑定点0上
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, nUboMatrices, 0, 2 * (sizeof(glm::mat4)));

	// 投影矩阵
	glm::mat4 mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, nUboMatrices);
	// 使用glBufferSubData函数，将数据更新到Uniform缓冲中
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mProjection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

		//glm::mat4 mProjection;
		//// 定义投影矩阵
		//mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		//// 定义观察矩阵
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

		//// 绘制4个顶点
		//glBindVertexArray(nPointVAO);
		//mModel = glm::mat4(1.0f);
		//mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, 0.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_POINTS, 0, 4);

		// 观察矩阵
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

		// 检查触发事件
		glfwPollEvents();
		// 交换颜色缓冲
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &nCubeVAO);
	//glDeleteVertexArrays(1, &nPointVAO);
	glDeleteBuffers(1, &nCubeVBO);
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
