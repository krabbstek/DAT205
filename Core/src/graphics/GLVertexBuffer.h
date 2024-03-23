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
		void Push(GLuint type, unsigned int count)
		{
			m_Layout.push_back(LayoutElement(type, count));
		}

		static unsigned int GetElementSize(GLuint type);

	private:
		/* std::pair< Element type , Element count > */
		std::vector<LayoutElement> m_Layout;

		friend class CORE_API GLVertexArray;
	};

}