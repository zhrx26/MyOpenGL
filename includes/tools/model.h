#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <tools/mesh.h>
#include <tools/shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char *sPath, const string &sDirectory);

class Model
{
public:
	vector<Mesh> m_vMeshes;
	vector<Texture> m_vVextures_loaded;
	Model(string const &sPath)
	{
		LoadModel(sPath);
	}
	void Draw(Shader pShader)
	{
		for (unsigned int i = 0; i < m_vMeshes.size(); i++)
			m_vMeshes[i].Draw(pShader);
	}
private:
	string m_sDirectory;  // 获取当前模型的目录字符串
	void LoadModel(string const &sPath)
	{
		Assimp::Importer pImport;
		const aiScene *pScene = pImport.ReadFile(sPath, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << pImport.GetErrorString() << endl;
			return;
		}

		m_sDirectory = sPath.substr(0, sPath.find_last_of('/'));
		ProcessNode(pScene->mRootNode, pScene);
	}
	void ProcessNode(aiNode *pNode, const aiScene *pScene)
	{
		// 处理节点所有的网格（如果有的话）
		for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
		{
			aiMesh *pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_vMeshes.push_back(ProcessMesh(pMesh, pScene));
		}
		// 接下来对它的子节点重复这一过程
		for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		{
			ProcessNode(pNode->mChildren[i], pScene);
		}
	}
	Mesh ProcessMesh(aiMesh *pMesh, const aiScene *pScene)
	{
		vector<Vertex> vVertices;
		vector<unsigned int> vIndices;
		vector<Texture> vTextures;
		// 从Assimp到网格
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			Vertex pVertex;
			glm::vec3 vVector;
			// 顶点位置
			vVector.x = pMesh->mVertices[i].x;
			vVector.y = pMesh->mVertices[i].y;
			vVector.z = pMesh->mVertices[i].z;
			pVertex.Position = vVector;
			// 法线
			vVector.x = pMesh->mNormals[i].x;
			vVector.y = pMesh->mNormals[i].y;
			vVector.z = pMesh->mNormals[i].z;
			pVertex.Normal = vVector;
			// 纹理坐标
			if (pMesh->mTextureCoords[0]) // 网格是否有纹理坐标？
			{
				glm::vec2 vec;
				vec.x = pMesh->mTextureCoords[0][i].x;
				vec.y = pMesh->mTextureCoords[0][i].y;
				pVertex.TexCoords = vec;
			}
			else
				pVertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vVertices.push_back(pVertex);
		}
		// 索引
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			aiFace pFace = pMesh->mFaces[i];
			for (unsigned int j = 0; j < pFace.mNumIndices; j++)
				vIndices.push_back(pFace.mIndices[j]);
		}
		// 材质
		if (pMesh->mMaterialIndex >= 0)
		{
			aiMaterial *pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
			// 加载漫反射材质
			vector<Texture> vDiffuseMaps = LoadMaterialTextures(pMaterial,
				aiTextureType_DIFFUSE, "texture_diffuse");
			vTextures.insert(vTextures.end(), vDiffuseMaps.begin(), vDiffuseMaps.end());
			// 加载镜面光材质
			vector<Texture> vSpecularMaps = LoadMaterialTextures(pMaterial,
				aiTextureType_SPECULAR, "texture_specular");
			vTextures.insert(vTextures.end(), vSpecularMaps.begin(), vSpecularMaps.end());

			//vector<Texture> vNormalMaps = LoadMaterialTextures(pMaterial,
			//	aiTextureType_HEIGHT, "texture_normal");
			//vTextures.insert(vTextures.end(), vNormalMaps.begin(), vNormalMaps.end());

			//vector<Texture> vHeightMaps = LoadMaterialTextures(pMaterial,
			//	aiTextureType_AMBIENT, "texture_height");
			//vTextures.insert(vTextures.end(), vHeightMaps.begin(), vHeightMaps.end());

			// Assimp在大多数格式中不支持反射贴图，所以需要欺骗它，将反射贴图储存为漫反射贴图
			vector<Texture> vReflectionMaps = LoadMaterialTextures(pMaterial,
				aiTextureType_AMBIENT, "texture_reflection");
			vTextures.insert(vTextures.end(), vReflectionMaps.begin(), vReflectionMaps.end());
		}

		return Mesh(vVertices, vIndices, vTextures);
	}
	vector<Texture> LoadMaterialTextures(aiMaterial *pMat, aiTextureType pType, string sTypeName)
	{
		vector<Texture> vTextures;
		// 遍历了给定纹理类型的所有纹理位置
		for (unsigned int i = 0; i < pMat->GetTextureCount(pType); i++)
		{
			aiString sStr;
			// 获取每个纹理的文件位置
			pMat->GetTexture(pType, i, &sStr);
			bool bSkip = false;
			// 遍历已加载的纹理队列，查看纹理是否有加载过的
			for (unsigned int j = 0; j < m_vVextures_loaded.size(); j++)
			{
				if (std::strcmp(m_vVextures_loaded[j].path.data(), sStr.C_Str()) == 0)
				{
					vTextures.push_back(m_vVextures_loaded[j]);
					bSkip = true;
					break;
				}
			}
			// 如果纹理还没有被加载，则加载它
			if (!bSkip)
			{
				Texture pTexture;
				// 加载一个纹理并返回该纹理的ID
				pTexture.id = TextureFromFile(sStr.C_Str(), m_sDirectory);
				pTexture.type = sTypeName;
				pTexture.path = sStr.C_Str();
				vTextures.push_back(pTexture);
				m_vVextures_loaded.push_back(pTexture);  // 添加到已加载的纹理队列中
			}

		}
		return vTextures;
	}
};


unsigned int TextureFromFile(const char *sPath, const string &sDirectory)
{
	string sFilename = string(sPath);
	sFilename = sDirectory + '/' + sFilename;

	unsigned int nTextureID;
	glGenTextures(1, &nTextureID);

	int nWidth, nHeight, nrComponents;
	unsigned char *sData = stbi_load(sFilename.c_str(), &nWidth, &nHeight, &nrComponents, 0);
	if (sData)
	{
		GLenum nFormat;
		if (nrComponents == 1)
			nFormat = GL_RED;
		else if (nrComponents == 3)
			nFormat = GL_RGB;
		else if (nrComponents == 4)
			nFormat = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, nTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, nFormat, nWidth, nHeight, 0, nFormat, GL_UNSIGNED_BYTE, sData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(sData);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << sPath << std::endl;
		stbi_image_free(sData);
	}

	return nTextureID;
}


#endif