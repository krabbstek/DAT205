#pragma once

#include "Common.h"

#include <GL/glew.h>
#include <unordered_map>

namespace core {

	class CORE_API GLShader
	{
	public:
		GLShader();
		~GLShader();

		inline void AddShader(GLuint shaderType, const char* shaderSource)
		{
			m_Shaders[shaderType] = shaderSource;
		}
		void AddShaderFromFile(GLuint shaderType, const char* filePath);
		bool CompileShaders();

		void Bind() const;
		void Unbind() const;

	private:
		GLuint m_RendererID;
		std::unordered_map<GLuint, const char*> m_Shaders;
		std::vector<char*> m_TempSource;

		static GLuint s_CurrentRendererID;
	};

}