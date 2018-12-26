#pragma once

#include "Common.h"

#include <GL/glew.h>

namespace core {

	class CORE_API GLVertexBuffer
	{
	public:
		GLVertexBuffer(const void* data, unsigned int size);
		~GLVertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		GLuint m_RendererID;

		static GLuint s_CurrentRendererID;
	};


	typedef std::pair<GLuint, unsigned int> LayoutElement;

	class CORE_API GLVertexBufferLayout
	{
	public:
		GLVertexBufferLayout();

		void Push(GLuint type, unsigned int count);

		static unsigned int GetElementSize(GLuint type);

	private:
		std::vector<LayoutElement> m_Layout;
		unsigned int m_Stride;

		friend class CORE_API GLVertexArray;
	};

}