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

//float lCubeVertices[] = {
//	// ����
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//
//	// ����
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//	// ����
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	// ����
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//	// ����
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//	// ����
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//};

float lCubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
};

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

float lPlaneVertices[] = {
	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
};

float lWindowVertices[] = {
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

float lSkyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

vector<glm::vec3> vWindowPos
{
	glm::vec3(-2.5f, 0.0f, 0.51f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-1.3f, 0.0f, -2.3f),
	glm::vec3(-0.5f, 0.0f, -3.49f),
};

vector<std::string> vFaces
{
	FileSystem::GetTexturePath("skybox/right.jpg"),
	FileSystem::GetTexturePath("skybox/left.jpg"),
	FileSystem::GetTexturePath("skybox/top.jpg"),
	FileSystem::GetTexturePath("skybox/bottom.jpg"),
	FileSystem::GetTexturePath("skybox/front.jpg"),
	FileSystem::GetTexturePath("skybox/back.jpg")
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
	glEnable(GL_CULL_FACE);

	Shader pShader("4.6.cubemaps.vs", "4.6.cubemaps.fs");
	Shader pSkyBoxShader("4.6.skybox.vs", "4.6.skybox.fs");

	Model pModel(FileSystem::GetObjectPath("nanosuit/nanosuit.obj"));

	//unsigned int nCubeVAO, nCubeVBO;
	//// �����������
	//glGenVertexArrays(1, &nCubeVAO);
	//glBindVertexArray(nCubeVAO);
	//// ���㻺�����
	//glGenBuffers(1, &nCubeVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, nCubeVBO);
	//// �󶨶������ݺͽ�����ʽ
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lCubeVertices), &lCubeVertices, GL_STATIC_DRAW);
	//// λ������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// ��������
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	////// ������������
	////glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	////glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//unsigned int nPlaneVAO, nPlaneVBO;
	//// �����������
	//glGenVertexArrays(1, &nPlaneVAO);
	//glBindVertexArray(nPlaneVAO);
	//// ���㻺�����
	//glGenBuffers(1, &nPlaneVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, nPlaneVBO);
	//// �󶨶������ݺͽ�����ʽ
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lPlaneVertices), lPlaneVertices, GL_STATIC_DRAW);
	//// λ������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// ������������
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//unsigned int nWindowVAO, nWindowVBO;
	//// �����������
	//glGenVertexArrays(1, &nWindowVAO);
	//glBindVertexArray(nWindowVAO);
	//// ���㻺�����
	//glGenBuffers(1, &nWindowVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, nWindowVBO);
	//// �󶨶������ݺͽ�����ʽ
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lWindowVertices), lWindowVertices, GL_STATIC_DRAW);
	//// λ������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// ������������
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	unsigned int nSkyBoxVAO, nSkyBoxVBO;
	// �����������
	glGenVertexArrays(1, &nSkyBoxVAO);
	glBindVertexArray(nSkyBoxVAO);
	// ���㻺�����
	glGenBuffers(1, &nSkyBoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nSkyBoxVBO);
	// �󶨶������ݺͽ�����ʽ
	glBufferData(GL_ARRAY_BUFFER, sizeof(lSkyboxVertices), &lSkyboxVertices, GL_STATIC_DRAW);
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int nModelVAO, nModelVBO;
	// �����������
	glGenVertexArrays(1, &nModelVAO);
	glBindVertexArray(nModelVAO);
	// ���㻺�����
	glGenBuffers(1, &nModelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nModelVBO);
	// �󶨶������ݺͽ�����ʽ
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), &lVertices, GL_STATIC_DRAW);
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ������������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//unsigned int nCubeTexture = LoadTexture(FileSystem::GetTexturePath("marble.jpg").c_str());
	//unsigned int nCubeTexture = LoadTexture(FileSystem::GetTexturePath("container.jpg").c_str());
	//unsigned int nPlaneTexture = LoadTexture(FileSystem::GetTexturePath("metal.png").c_str());
	//unsigned int nWindowTexture = LoadTexture(FileSystem::GetTexturePath("window.png").c_str());

	unsigned int nCubemapTexture = LoadCubeMaps(vFaces);

	//pShader.Use();
	//pShader.SetInt("textureID", 0);
	//pShader.SetInt("skybox", 0);

	//pSkyBoxShader.Use();
	//pSkyBoxShader.SetInt("skybox", 0);

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

		glm::mat4 mProjection;
		// ����ͶӰ����
		mProjection = glm::perspective(glm::radians(pCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		// ����۲����
		glm::mat4 mView;
		mView = pCamera.GetViewMatrix();
		glm::mat4 mModel;

		glActiveTexture(GL_TEXTURE0);

		pShader.Use();
		pShader.SetMat4("view", mView);
		pShader.SetMat4("projection", mProjection);
		pShader.SetVec3("cameraPos", pCamera.Position);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, nCubemapTexture);
		//glBindVertexArray(nCubeVAO);
		//glBindTexture(GL_TEXTURE_2D, nCubeTexture);
		//mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, -4.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//mModel = glm::mat4(1.0f);
		//mModel = glm::translate(mModel, glm::vec3(-2.0f, 0.0f, 0.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// ��Ⱦģ��
		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, 0.0f));
		mModel = glm::scale(mModel, glm::vec3(0.2f, 0.2f, 0.2f));
		glBindVertexArray(nModelVAO);
		pShader.SetMat4("model", mModel);
		// ��Ϊģ���Ѿ�ռ����3��ͨ������պ��õ�4��
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, nCubemapTexture);
		pShader.SetInt("skybox", 4);
		pModel.Draw(pShader);
		glActiveTexture(GL_TEXTURE0);

		//// ����Ƭ�ر����޳�����Ȼ��ͷŲ�������ʱ��ᱻ�޳���
		//glDisable(GL_CULL_FACE);

		//glBindVertexArray(nPlaneVAO);
		//glBindTexture(GL_TEXTURE_2D, nPlaneTexture);
		//mModel = glm::mat4(1.0f);
		//mModel = glm::translate(mModel, glm::vec3(0.0f, -0.01f, 0.0f));
		//pShader.SetMat4("model", mModel);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		// ��Ⱦ��պУ�Ҫ��͸������֮ǰ��Ⱦ
		pSkyBoxShader.Use();
		// ͨ��ȡ4x4�������Ͻǵ�3x3�������Ƴ��任�����λ�Ʋ���
		pSkyBoxShader.SetMat4("view", glm::mat4(glm::mat3(mView)));
		pSkyBoxShader.SetMat4("projection", mProjection);
		// ��Ƭ�����ֵС�ڵ��ڻ����������ֵʱͨ����Ȳ���
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(nSkyBoxVAO);
		// ��Ϊģ���Ѿ�ռ����3��ͨ������պ��õ�4��
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, nCubemapTexture);
		pSkyBoxShader.SetInt("skybox", 4);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// ��Ƭ�����ֵС�ڻ�������ֵʱͨ����Ȳ���
		glDepthFunc(GL_LESS);
		glActiveTexture(GL_TEXTURE0);

		//pShader.Use();
		//glBindVertexArray(nWindowVAO);
		//glBindTexture(GL_TEXTURE_2D, nWindowTexture);
		//// ������͸���Ĵ�����ͼ��������
		//std::map<float, glm::vec3> mSorted;
		//for (unsigned int i = 0; i < vWindowPos.size(); i++)
		//{
		//	// �þ���������˵õ��ľ�������keyֵ�Ļ����Ա���sqrt����
		//	glm::vec3 vDistance = pCamera.Position - vWindowPos[i];
		//	float fDistance = glm::dot(vDistance, vDistance);
		//	mSorted[fDistance] = vWindowPos[i];
		//}
		//// �����򣨴�Զ��������map�л�ȡֵ,���ƶ�Ӧ�Ĵ���
		//for (std::map<float, glm::vec3>::reverse_iterator it = mSorted.rbegin(); it != mSorted.rend(); ++it)
		//{
		//	mModel = glm::mat4(1.0);
		//	mModel = glm::translate(mModel, it->second);
		//	pShader.SetMat4("model", mModel);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		glBindVertexArray(0);
		//// ���¿������޳�
		//glEnable(GL_CULL_FACE);

		// ��鴥���¼�
		glfwPollEvents();
		// ������ɫ����
		glfwSwapBuffers(pWindow);
	}

	//glDeleteVertexArrays(1, &nCubeVAO);
	//glDeleteVertexArrays(1, &nPlaneVAO);
	//glDeleteVertexArrays(1, &nWindowVAO);
	glDeleteVertexArrays(1, &nSkyBoxVAO);
	glDeleteVertexArrays(1, &nModelVAO);
	//glDeleteBuffers(1, &nCubeVBO);
	//glDeleteBuffers(1, &nPlaneVBO);
	//glDeleteBuffers(1, &nWindowVBO);
	glDeleteBuffers(1, &nSkyBoxVBO);
	glDeleteBuffers(1, &nModelVBO);

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

/*
����Ŀ�� 						��λ
GL_TEXTURE_CUBE_MAP_POSITIVE_X 	��
GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	��
GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	��
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	��
GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	��
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	ǰ
*/
unsigned int LoadCubeMaps(vector<std::string> vFace)
{
	int nWidth, nHeight, nrChannels;
	unsigned int nTextureID;
	glGenTextures(1, &nTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, nTextureID);
	for (unsigned int i = 0;i < vFace.size(); i++)
	{
		unsigned char *sImage = stbi_load(vFace[i].c_str(), &nWidth, &nHeight, &nrChannels, 0);
		if (sImage)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, nWidth, nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sImage);
			stbi_image_free(sImage);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << vFaces[i] << std::endl;
			stbi_image_free(sImage);
		}
	}
	// �趨���ƺ͹��˷�ʽ
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return nTextureID;
}
