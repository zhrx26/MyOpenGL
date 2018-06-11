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
void ProcessInput(GLFWwindow * pWindow);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

float lVertices[] = {
	0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
};

unsigned int lIndices[] = {
	0, 1, 3,
	1, 2, 3,
};

float fMixValue = 0.2f;

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

	// ������װ����Shader����
	Shader pShader("1.5.transformations.vs", "1.5.transformations.fs");

	unsigned int nVAO, nVBO, nEBO;
	// �����������
	glGenVertexArrays(1, &nVAO);
	glBindVertexArray(nVAO);
	// ���㻺�����
	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);

	//�����������
	glGenBuffers(1, &nEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lIndices), lIndices, GL_STATIC_DRAW);
	// �󶨶������ݺͽ�����ʽ
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_STATIC_DRAW);

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ��ΪOpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ���
	// �����ڼ����κ�ͼ��֮ǰ����תy��
	stbi_set_flip_vertically_on_load(true);
	// ��������
	unsigned int nTexture;
	glGenTextures(1, &nTexture);
	glBindTexture(GL_TEXTURE_2D, nTexture);

	// S��T���Ӧx��y��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ��Сʱ�������ڽ��Ķ༶��Զ����֮��ʹ�����Բ�ֵ����ʹ�����Բ�ֵ���в���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nWidth, nHeight, nrChannels;
	// ����ͼ���ļ�
	unsigned char *sImage = stbi_load(FileSystem::GetTexturePath("container.jpg").c_str(), &nWidth, &nHeight, &nrChannels, 0);
	if (sImage)
	{
		// ����ͼƬ��������һ������
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
	// ����ͼ���ļ�
	sImage = stbi_load(FileSystem::GetTexturePath("awesomeface.png").c_str(), &nWidth, &nHeight, &nrChannels, 0);
	if (sImage)
	{
		// ����ͼƬ��������һ������
		// ע�⣡PNG��ʽ��ͼƬ��4��ͨ����Ҫʹ��GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sImage);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(sImage);

	pShader.Use();
	pShader.SetInt("texture1", 0);
	pShader.SetInt("texture2", 1);

	//glm::vec4 vCur(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 mTrans;
	//mTrans = glm::translate(mTrans, glm::vec3(1.0f, 1.0f, 0.0f));
	//vCur = mTrans * vCur;
	//std::cout << vCur.x << vCur.y << vCur.z << std::endl;

	// ѭ����Ⱦ
	while (!glfwWindowShouldClose(pWindow))
	{
		// �������
		ProcessInput(pWindow);
		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, nTexture2);


		glm::mat4 mTrans;
		//mTrans = glm::rotate(mTrans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//mTrans = glm::scale(mTrans, glm::vec3(0.5f, 0.5f, 0.5f));

		pShader.Use();
		pShader.SetFloat("fMixValue", fMixValue);
		// ��λ���ٸ���ʱ����ת
		mTrans = glm::translate(mTrans, glm::vec3(0.5f, -0.5f, 0.0f));

		//std::cout << (float*)glm::value_ptr(mTrans) << std::endl;

		mTrans = glm::rotate(mTrans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		//unsigned int nTransLoc = glGetUniformLocation(pShader.m_nID, "transform");
		//glUniformMatrix4fv(nTransLoc, 1, GL_FALSE, glm::value_ptr(mTrans));

		pShader.SetMat4("transform", mTrans);

		glBindVertexArray(nVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		mTrans = glm::mat4(1.0f);
		// ��λ���ٸ���ʱ������
		mTrans = glm::translate(mTrans, glm::vec3(-0.5f, 0.5f, 0.0f));
		mTrans = glm::scale(mTrans, glm::vec3(sin((float)glfwGetTime())));
		pShader.SetMat4("transform", mTrans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	else if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
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
