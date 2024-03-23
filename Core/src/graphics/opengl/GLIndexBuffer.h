#pragma once

#include "Common.h"

#include <GL/glew.h>

namespace core {

	class CORE_API GLIndexBuffer
	{
	public:
		GLIndexBuffer(const unsigned int* data, unsigned int count);
		~GLIndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int Count() const { return m_Count; }

	private:
		GLuint m_RendererID;
		unsigned int m_Count;
	};

}