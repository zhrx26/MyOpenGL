#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>

#include<tools/filesystem.h>
#include<tools/shader.h>
#include<tools/camera.h>

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

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

bool bFirstMouse = true;
float fDeltaTime = 0.0f; // 当前帧与上一帧的时间差
float fLastFrame = 0.0f; // 上一帧的时间

Camera pCamera(glm::vec3(0.0f, 1.0f, 5.0f));


float fLastMouseX = WINDOW_WIDTH / 2.0f;
float fLastMouseY = WINDOW_HEIGHT / 2.0f;

float lCubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float lPlaneVertices[] = {
	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

float lGrassVertices[] = {
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

vector<glm::vec3> vGrassPos
{
	glm::vec3(-2.5f, 0.0f, 0.51f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-1.3f, 0.0f, -2.3f),
	glm::vec3(-0.5f, 0.0f, -3.49f),
};

vector<glm::vec3> vWindowPos
{
	glm::vec3(-2.5f, 0.0f, 0.51f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-1.3f, 0.0f, -2.3f),
	glm::vec3(-0.5f, 0.0f, -3.49f),
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

	Shader pShader("4.3.blending.vs", "4.3.blending.fs");

	unsigned int nCubeVAO, nCubeVBO;
	// 顶点数组对象
	glGenVertexArrays(1, &nCubeVAO);
	glBindVertexArray(nCubeVAO);
	// 顶点缓冲对象
	glGenBuffers(1, &nCubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nCubeVBO);
	// 绑定顶点数据和解析格式
	glBufferData(GL_ARRAY_BUFFER, sizeof(lCubeVertices), lCubeVertices, GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int nPlaneVAO, nPlaneVBO;
	// 顶点数组对象
	glGenVertexArrays(1, &nPlaneVAO);
	glBindVertexArray(nPlaneVAO);
	// 顶点缓冲对象
	glGenBuffers(1, &nPlaneVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nPlaneVBO);
	// 绑定顶点数据和解析格式
	glBufferData(GL_ARRAY_BUFFER, sizeof(lPlaneVertices), lPlaneVertices, GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//unsigned int nGrassVAO, nGrassVBO;
	//// 顶点数组对象
	//glGenVertexArrays(1, &nGrassVAO);
	//glBindVertexArray(nGrassVAO);
	//// 顶点缓冲对象
	//glGenBuffers(1, &nGrassVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, nGrassVBO);
	//// 绑定顶点数据和解析格式
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lGrassVertices), lGrassVertices, GL_STATIC_DRAW);
	//// 位置属性
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// 纹理坐标属性
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	unsigned int nWindowVAO, nWindowVBO;
	// 顶点数组对象
	glGenVertexArrays(1, &nWindowVAO);
	glBindVertexArray(nWindowVAO);
	// 顶点缓冲对象
	glGenBuffers(1, &nWindowVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nWindowVBO);
	// 绑定顶点数据和解析格式
	glBufferData(GL_ARRAY_BUFFER, sizeof(lGrassVertices), lGrassVertices, GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int nCubeTexture = LoadTexture(FileSystem::GetTexturePath("marble.jpg").c_str());
	unsigned int nPlaneTexture = LoadTexture(FileSystem::GetTexturePath("metal.png").c_str());
	//unsigned int nGrassTexture = LoadTexture(FileSystem::GetTexturePath("grass.png").c_str());
	unsigned int nWindowTexture = LoadTexture(FileSystem::GetTexturePath("window.png").c_str());

	pShader.Use();
	pShader.SetInt("textureID", 0);

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

		pShader.Use();
		glm::mat4 mProjection;
		// 定义投影矩阵
		mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		// 定义观察矩阵
		glm::mat4 mView;
		mView = pCamera.GetViewMatrix();
		glm::mat4 mModel;

		pShader.SetMat4("view", mView);
		pShader.SetMat4("projection", mProjection);

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(nCubeVAO);
		glBindTexture(GL_TEXTURE_2D, nCubeTexture);
		mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, -4.0f));
		pShader.SetMat4("model", mModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(-2.0f, 0.0f, 0.0f));
		pShader.SetMat4("model", mModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(nPlaneVAO);
		glBindTexture(GL_TEXTURE_2D, nPlaneTexture);
		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(0.0f, -0.01f, 0.0f));
		pShader.SetMat4("model", mModel);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 绘制半透明的草纹理
		//glBindVertexArray(nGrassVAO);
		//glBindTexture(GL_TEXTURE_2D, nGrassTexture);
		//for (unsigned int i = 0; i < vGrassPos.size(); i++)
		//{
		//	mModel = glm::mat4(1.0f);
		//	mModel = glm::translate(mModel, vGrassPos[i]);
		//	pShader.SetMat4("model", mModel);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		glBindVertexArray(nWindowVAO);
		glBindTexture(GL_TEXTURE_2D, nWindowTexture);
		//for (unsigned int i = 0; i < vWindowPos.size(); i++)
		//{
		//	mModel = glm::mat4(1.0f);
		//	mModel = glm::translate(mModel, vWindowPos[i]);
		//	pShader.SetMat4("model", mModel);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		// 对所有透明的窗口贴图进行排序
		std::map<float, glm::vec3> mSorted;
		for (unsigned int i = 0; i < vWindowPos.size(); i++)
		{
			//float fDistance = glm::length(pCamera.Position - vWindowPos[i]);

			// 用距离向量点乘得到的距离来做key值的话可以避免sqrt运算
			glm::vec3 vDistance = pCamera.Position - vWindowPos[i];
			float fDistance = glm::dot(vDistance, vDistance);
			mSorted[fDistance] = vWindowPos[i];
		}
		// 以逆序（从远到近）从map中获取值,绘制对应的窗户
		for (std::map<float, glm::vec3>::reverse_iterator it = mSorted.rbegin(); it != mSorted.rend(); ++it)
		{
			mModel = glm::mat4(1.0);
			mModel = glm::translate(mModel, it->second);
			pShader.SetMat4("model", mModel);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);

		// 检查触发事件
		glfwPollEvents();
		// 交换颜色缓冲
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &nCubeVAO);
	glDeleteVertexArrays(1, &nPlaneVAO);
	//glDeleteVertexArrays(1, &nGrassVAO);
	glDeleteVertexArrays(1, &nWindowVAO);
	glDeleteBuffers(1, &nCubeVBO);
	glDeleteBuffers(1, &nPlaneVBO);
	//glDeleteBuffers(1, &nGrassVBO);
	glDeleteBuffers(1, &nWindowVBO);

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
