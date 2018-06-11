#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<tools/filesystem.h>
#include<tools/shader.h>

#include<stb_image.h>

void FramebufferSizeCallback(GLFWwindow * pWindow, int nWidth, int nHeight);
void ProcessInput(GLFWwindow * pWindow);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

//float lVertices[] = {
//	0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
//	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
//	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
//	-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
//};

float lVertices[] = {
	0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  2.0f, 2.0f,
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 2.0f,
};

unsigned int lIndices[] = {
	0, 1, 3,
	1, 2, 3,
};

float fMixValue = 0.2f;

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
	// 设置窗口大小变换时的回调函数
	glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);

	// 创建封装过的Shader对象
	Shader pShader("1.4.textures.vs", "1.4.textures.fs");

	unsigned int nVAO, nVBO, nEBO;
	// 顶点数组对象
	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);
	// 顶点缓冲对象
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);

	//索引缓冲对象
	glGenBuffers(1, &nEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lIndices), lIndices, GL_STATIC_DRAW);
	// 绑定顶点数据和解析格式
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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

	// 超出的坐标为用户指定的边缘颜色
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//float lBorderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, lBorderColor);

	// 缩小时在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// 缩小时采用邻近过滤
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 放大时采用线性过滤
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
		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, nTexture2);

		pShader.Use();
		glBindVertexArray(nVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		pShader.SetFloat("fMixValue", fMixValue);

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
	else if(glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		fMixValue += 0.01f;
		if (fMixValue > 1.0f)
			fMixValue = 1.0f;
	}
	else if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		fMixValue -= 0.01f;
		if (fMixValue < 0.0f)
			fMixValue = 0.0f;
	}
}
