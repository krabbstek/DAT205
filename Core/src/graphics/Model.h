#pragma once

#include "Common.h"

#include "opengl/GLVertexArray.h"
#include "opengl/GLIndexBuffer.h"

#include <GL/glew.h>

namespace core {

	class CORE_API Model
	{
	public:
		Model();
		~Model();

		static Model* LoadOBJModelFromFile(const char* filePath);

	private:
		GLVertexArray* m_VAO;
		GLIndexBuffer* m_IBO;
	};

}