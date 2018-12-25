#include "pch.h"

#include "GLShader.h"

#include "GLCommon.h"

namespace core {

	GLuint GLShader::s_CurrentRendererID = 0;


	GLShader::GLShader()
		: m_RendererID(0)
	{
	}

	GLShader::~GLShader()
	{
		if (m_RendererID)
		{
			if (s_CurrentRendererID == m_RendererID)
			{
				GLCall(glUseProgram(0));
				s_CurrentRendererID = 0;
			}
			GLCall(glDeleteProgram(m_RendererID));
		}
	}


	void GLShader::AddShaderFromFile(GLuint shaderType, const char* filePath)
	{
		std::FILE* file = std::fopen(filePath, "rb");
		if (file)
		{
			std::string source;
			std::fseek(file, 0, SEEK_END);
			unsigned int size = std::ftell(file);
			source.resize(size);
			std::rewind(file);
			std::fread(&source[0], 1, size, file);
			std::fclose(file);

			char* c = new char[source.size() + 1];
			memcpy(c, &source[0], source.size());
			c[source.size()] = 0;
			m_TempSource.push_back(c);

			AddShader(shaderType, c);
		}
		else
			CORE_ERROR("AddShaderFromFile - failed to open file \"{}\"", filePath);
	}

	bool GLShader::CompileShaders()
	{
		std::vector<GLuint> compiledShaders;
		GLint result = 0;
		GLint length = 0;
		constexpr GLint maxLength = 256;
		char info[maxLength];

		for (auto a : m_Shaders)
		{
			GLCall(GLuint shader = glCreateShader(a.first));
			GLCall(glShaderSource(shader, 1, &a.second, 0));
			GLCall(glCompileShader(shader));
			
			GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
			if (result == GL_FALSE)
			{
				GLCall(glGetShaderInfoLog(shader, maxLength, &length, info));
				GLCall(glDeleteShader(shader));
				for (GLuint sh : compiledShaders)
				{
					GLCall(glDeleteShader(sh));
				}
				CORE_ERROR("Failed to compile shader (type {}): {}", a.first, info);
				return false;
			}

			compiledShaders.push_back(shader);
		}

		GLCall(m_RendererID = glCreateProgram());
		for (GLuint shader : compiledShaders)
		{
			GLCall(glAttachShader(m_RendererID, shader));
		}
		GLCall(glLinkProgram(m_RendererID));

		GLCall(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &result));
		if (result == GL_FALSE)
		{
			GLCall(glGetProgramInfoLog(m_RendererID, length, &length, &info[0]));
			GLCall(glDeleteProgram(m_RendererID));
			m_RendererID = 0;
			for (GLuint shader : compiledShaders)
			{
				GLCall(glDeleteShader(shader));
			}
			CORE_ERROR("Failed to link shaders: {}", info);
			return false;
		}

		for (GLuint shader : compiledShaders)
		{
			GLCall(glDetachShader(m_RendererID, shader));
		}
		while (m_TempSource.size())
		{
			delete m_TempSource.back();
			m_TempSource.pop_back();
		}

		return true;
	}


	void GLShader::Bind() const
	{
		if (m_RendererID != s_CurrentRendererID)
		{
			GLCall(glUseProgram(m_RendererID));
			s_CurrentRendererID = m_RendererID;
		}
	}

	void GLShader::Unbind() const
	{
		if (s_CurrentRendererID)
		{
			GLCall(glUseProgram(0));
			s_CurrentRendererID = 0;
		}
	}


	unsigned int GLShader::GetUniformLocation(std::string& uniformName)
	{
		auto i = m_UniformLocations.find(uniformName);
		// Uniform location not found in map
		if (i == m_UniformLocations.end())
		{
			GLCall(unsigned int location = glGetUniformLocation(m_RendererID, uniformName.c_str()));
			m_UniformLocations[uniformName] = location;
			return location;
		}
		// Uniform location found in map
		else
			return i->second;
	}

}