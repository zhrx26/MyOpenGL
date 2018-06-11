#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tools/shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;  // ���������·������������������бȽ�
};

class Mesh {

public:
	vector<Vertex> m_vVertices;
	vector<unsigned int> m_vIndices;
	vector<Texture> m_vTextures;
	unsigned int m_nVAO, m_nVBO, m_nEBO;
	Mesh(vector<Vertex> vVertices, vector<unsigned int> vIndices, vector<Texture> vTextures)
	{
		this->m_vVertices = vVertices;
		this->m_vIndices = vIndices;
		this->m_vTextures = vTextures;
		// ��ʼ����������
		InitMesh();
	}
	void Draw(Shader pShader)
	{
		unsigned int nDiffuseNr = 1;
		unsigned int nSpecularNr = 1;
		//unsigned int nNormalNr = 1;
		//unsigned int nHeightNr = 1;
		unsigned int nReflectionNr = 1;
		for (unsigned int i = 0; i < m_vTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			string sNumber;
			string sName = m_vTextures[i].type;
			// ���������
			if (sName == "texture_diffuse")
				sNumber = std::to_string(nDiffuseNr++);
			// ��������
			else if (sName == "texture_specular")
				sNumber = std::to_string(nSpecularNr++);

			//else if (sName == "texture_normal")
			//	sNumber = std::to_string(nNormalNr++);

			//else if (sName == "texture_height")
			//	sNumber = std::to_string(nHeightNr++);

			// ������ͼ
			else if (sName == "texture_reflection")
				sNumber = std::to_string(nReflectionNr++);

			pShader.SetInt("material." + sName + sNumber.c_str(), i);
			glBindTexture(GL_TEXTURE_2D, m_vTextures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// ��������
		glBindVertexArray(m_nVAO);
		glDrawElements(GL_TRIANGLES, m_vIndices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

private:
	void InitMesh()
	{
		// �����������
		glGenVertexArrays(1, &m_nVAO);
		glBindVertexArray(m_nVAO);
		// ���㻺�����
		glGenBuffers(1, &m_nVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);

		// ������������
		glGenBuffers(1, &m_nEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

		// �󶨶������ݺͽ�����ʽ
		glBufferData(GL_ARRAY_BUFFER, m_vVertices.size() * sizeof(Vertex), &m_vVertices[0], GL_STATIC_DRAW);
		
		// ���������ݺͽ�����ʽ
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndices.size() * sizeof(unsigned int), &m_vIndices[0], GL_STATIC_DRAW);

		// ����λ��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		// ���㷨��
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);
		// ������������
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};
#endif