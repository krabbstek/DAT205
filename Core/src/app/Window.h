#pragma once

#include "Common.h"

#include "math/vec3.h"
#include "math/vec4.h"

#include <string>

struct GLFWwindow;

namespace core {

	class CORE_API Window
	{
	public:
		Window(const char* windowTitle, unsigned int width, unsigned int height, bool vSync = true);
		~Window();

		void Update(float deltaTime);
		void Render();
		void Clear();

		void SetClearColor(const vec3& clearColor) const;
		void SetClearColor(const vec4& clearColor) const;

	private:
		std::string m_WindowTitle;
		unsigned int m_Width, m_Height;
		bool m_VSync;
		GLFWwindow* m_Window;

		friend CORE_API class Application;
	};

}