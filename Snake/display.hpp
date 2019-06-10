#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace snake {
	class Display {
		bool isFullScreen;
	
	public:
		Display(int width, int height, const std::string& title);

		void Clear(float r, float g, float b, float a);
		void SwapBuffers();
		bool IsFullscreen();

		virtual ~Display();

		GLFWwindow* m_window;
		int error;
	};
}