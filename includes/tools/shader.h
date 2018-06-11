#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int m_nID;
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        std::string sVertexCode;
        std::string sFragmentCode;
		std::string sGeometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
		// 保证ifstream对象可以抛出异常
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
			// 写入顶点着色器和片段着色器文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();	
            vShaderFile.close();
            fShaderFile.close();
            sVertexCode = vShaderStream.str();
            sFragmentCode = fShaderStream.str();
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				sGeometryCode = gShaderStream.str();
			}
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Failed to read shader file" << std::endl;
        }
        const char* sVertexShaderSource = sVertexCode.c_str();
        const char * sFragmentShaderSource = sFragmentCode.c_str();

		unsigned int nVertexShader, nFragmentShader, nGeometryShader;
		int nSuccess;
		char sInfoLog[512];
		// 顶点着色器
		nVertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(nVertexShader, 1, &sVertexShaderSource, NULL);
		glCompileShader(nVertexShader);
		glGetShaderiv(nVertexShader, GL_COMPILE_STATUS, &nSuccess);
		if (!nSuccess)
		{
			glGetShaderInfoLog(nVertexShader, 512, NULL, sInfoLog);
			std::cout << "Failed to compile vertex shader" << sInfoLog  << std::endl;
		}
		// 片段着色器
		nFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(nFragmentShader, 1, &sFragmentShaderSource, NULL);
		glCompileShader(nFragmentShader);
		glGetShaderiv(nFragmentShader, GL_COMPILE_STATUS, &nSuccess);
		if (!nSuccess)
		{
			glGetShaderInfoLog(nFragmentShader, 512, NULL, sInfoLog);
			std::cout << "Failed to compile fragment shader" << sInfoLog << std::endl;
		}
		// 几何着色器（如果有的话）
		if (geometryPath != nullptr)
		{
			const char * sGeometryShaderSource = sGeometryCode.c_str();
			nGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(nGeometryShader, 1, &sGeometryShaderSource, NULL);
			glCompileShader(nGeometryShader);
			glGetShaderiv(nGeometryShader, GL_COMPILE_STATUS, &nSuccess);
			if (!nSuccess)
			{
				glGetShaderInfoLog(nGeometryShader, 512, NULL, sInfoLog);
				std::cout << "Failed to compile geometry shader" << sInfoLog << std::endl;
			}
		}
		// 着色器程序
		m_nID = glCreateProgram();
		glAttachShader(m_nID, nVertexShader);
		glAttachShader(m_nID, nFragmentShader);
		if (geometryPath != nullptr)
			glAttachShader(m_nID, nGeometryShader);
		glLinkProgram(m_nID);
		glGetShaderiv(m_nID, GL_COMPILE_STATUS, &nSuccess);
		if (!nSuccess)
		{
			glGetProgramInfoLog(m_nID, 512, NULL, sInfoLog);
			std::cout << "Failed to link shader program" << sInfoLog << std::endl;
		}
		// 释放已经绑定的着色器
        glDeleteShader(nVertexShader);
        glDeleteShader(nFragmentShader);
		if (geometryPath != nullptr)
			glDeleteShader(nGeometryShader);
    }
    // ------------------------------------------------------------------------
    void Use() 
    { 
        glUseProgram(m_nID); 
    }
    // ------------------------------------------------------------------------
    void SetBool(const std::string &sName, bool value) const
    {         
        glUniform1i(glGetUniformLocation(m_nID, sName.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void SetInt(const std::string &sName, int value) const
    { 
        glUniform1i(glGetUniformLocation(m_nID, sName.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void SetFloat(const std::string &sName, float value) const
    { 
        glUniform1f(glGetUniformLocation(m_nID, sName.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void SetVec2(const std::string &sName, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(m_nID, sName.c_str()), 1, &value[0]); 
    }
    void SetVec2(const std::string &sName, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(m_nID, sName.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void SetVec3(const std::string &sName, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(m_nID, sName.c_str()), 1, &value[0]); 
    }
    void SetVec3(const std::string &sName, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(m_nID, sName.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void SetVec4(const std::string &sName, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(m_nID, sName.c_str()), 1, &value[0]); 
    }
    void SetVec4(const std::string &sName, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(m_nID, sName.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void SetMat2(const std::string &sName, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_nID, sName.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMat3(const std::string &sName, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_nID, sName.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void SetMat4(const std::string &sName, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_nID, sName.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
#endif