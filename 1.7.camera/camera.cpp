#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>

#include<tools/filesystem.h>
#include<tools/shader.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>

void FramebufferSizeCallback(GLFWwindow * pWindow, int nWidth, int nHeight);
void MouseMoveCallback(GLFWwindow* pWindow, double xPos, double yPos);
void MouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset);
void ProcessInput(GLFWwindow * pWindow);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

float fMixValue = 0.2f;
bool bFirstMouse = true;
float fDeltaTime = 0.0f; // 当前帧与上一帧的时间差
float fLastFrame = 0.0f; // 上一帧的时间

float fYaw = -90.0f;
float fPitch = 0.0f;
float fFov = 45.0f;  // 透视投影矩阵上的视野值

float fLastMouseX = WINDOW_WIDTH / 2.0f;
float fLastMouseY = WINDOW_HEIGHT / 2.0f;

float lVertices[] = {
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

glm::vec3 lCubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
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
	Shader pShader("1.7.camera.vs", "1.7.camera.fs");

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部
	// 所以在加载任何图像之前，翻转y轴
	stbi_set_flip_vertically_on_load(true);
	// 创建纹理
	unsigned int nTexture;
	glGenTextures(1, &nTexture);
	glBindTexture(GL_TEXTURE_2D, nTexture);

	// S和T轴对应x和y轴
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 缩小时在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nWidth, nHeight, nrChannels;
	// 输入图像文件
	unsigned char *sImage = stbi_load(FileSystem::GetTexturePath("container.jpg").c_str(), &nWidth, &nHeight, &nrChannels, 0);
	if (sImage)
	{
		// 载入图片数据生成一个纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sImage);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(sImage);

	unsigned int nTexture2;
	glGenTextures(1, &nTexture2);
	glBindTexture(GL_TEXTURE_2D, nTexture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 输入图像文件
	sImage = stbi_load(FileSystem::GetTexturePath("awesomeface.png").c_str(), &nWidth, &nHeight, &nrChannels, 0);
	if (sImage)
	{
		// 载入图片数据生成一个纹理
		// 注意！PNG格式的图片有4个通道，要使用GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sImage);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(sImage);

	pShader.Use();
	pShader.SetInt("texture1", 0);
	pShader.SetInt("texture2", 1);


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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, nTexture2);

		pShader.Use();
		pShader.SetFloat("fMixValue", fMixValue);

		//glm::mat4 mView;
		//// 将矩阵向场景的反方向移动
		//mView = glm::translate(mView, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 mProjection;
		// 定义投影矩阵
		mProjection = glm::perspective(glm::radians(fFov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		//glm::vec3 vCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		//glm::vec3 vCameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		// 摄像机方向是用摄像机所在点和目标点相减得出的
		//glm::vec3 vCameraDirection = glm::normalize(vCameraPos - vCameraTarget);
		//glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);
		// 摄像机的右轴是用上向量和摄像机方向叉乘得出来的，标准化向量可以实现匀速运动
		//glm::vec3 vCameraRight = glm::normalize(glm::cross(vUp, vCameraDirection));
		// 摄像机的上轴，用上向量和右轴叉乘得出
		//glm::vec3 vCameraUp = glm::cross(vCameraDirection, vCameraRight);

		glm::mat4 mView;
		// lookAt函数

		//mView = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f));

		//float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//glm::mat4 mView;
		//mView = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));


		mView = glm::lookAt(vCameraPos, vCameraPos + vCameraFront, vCameraUp);

		pShader.SetMat4("view", mView);
		pShader.SetMat4("projection", mProjection);


		glBindVertexArray(nVAO);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 mModel;
			mModel = glm::translate(mModel, lCubePositions[i]);
			float fAngle = 20.0f * i;
			mModel = glm::rotate(mModel, glm::radians(fAngle), glm::vec3(1.0f, 0.3f, 0.5f));
			pShader.SetMat4("model", mModel);
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
	//float fCameraSpeed = 0.05f;
	float fCameraSpeed = 2.5f * fDeltaTime;
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, true);
	if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		fMixValue += 0.01f;
		if (fMixValue > 1.0f)
			fMixValue = 1.0f;
	}
	if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		fMixValue -= 0.01f;
		if (fMixValue < 0.0f)
			fMixValue = 0.0f;
	}
	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
		vCameraPos += fCameraSpeed * vCameraFront;
	if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
		vCameraPos -= fCameraSpeed * vCameraFront;
	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
		vCameraPos -= glm::normalize(glm::cross(vCameraFront, vCameraUp)) * fCameraSpeed;
	if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
		vCameraPos += glm::normalize(glm::cross(vCameraFront, vCameraUp)) * fCameraSpeed;
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

	float sensitivity = 0.05f;
	fXOffset *= sensitivity;
	fYOffset *= sensitivity;

	fYaw += fXOffset;
	fPitch += fYOffset;
	// 在俯仰超过90度时视角会发生逆转
	if (fPitch > 89.0f)
		fPitch = 89.0f;
	if (fPitch < -89.0f)
		fPitch = -89.0f;

	glm::vec3 vFront;
	vFront.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vFront.y = sin(glm::radians(fPitch));
	vFront.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vCameraFront = glm::normalize(vFront);

}


void MouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
	if (fFov >= 1.0f && fFov <= 45.0f)
		fFov -= yOffset;
	if (fFov <= 1.0f)
		fFov = 1.0f;
	if (fFov >= 45.0f)
		fFov = 45.0f;
}
