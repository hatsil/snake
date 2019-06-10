#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include "display.hpp"


//void APIENTRY
//MessageCallback(GLenum source,
//	GLenum type,
//	GLuint id,
//	GLenum severity,
//	GLsizei length,
//	const GLchar* message,
//	GLvoid* userParam) {
//	if(type == GL_DEBUG_TYPE_ERROR)
//		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
//			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
//			type, severity, message);
//}

snake::Display::Display(int width, int height, const std::string& title) {
	/* Initialize the library */
	if (!glfwInit())
		error = -1;

	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		error = -1;
	}
	glfwMakeContextCurrent(m_window);
	//m_glContext = SDL_GL_CreateContext(m_window);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << std::endl;
	}
	isFullScreen = false;
	glEnable(GL_DEPTH_TEST);
	// During init, enable debug output
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);
	//glEnable(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT_AND_BACK);
}

snake::Display::~Display() {
	//SDL_GL_DeleteContext(m_glContext);
	glfwDestroyWindow(m_window);
	glfwTerminate();
	//SDL_Quit();
}

void snake::Display::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool snake::Display::IsFullscreen() {
	if ((glfwGetWindowMonitor(m_window) == nullptr) != isFullScreen)
	{
		isFullScreen = (glfwGetWindowMonitor(m_window) == nullptr);
		return true;
	}
	else
		return false;
}

void snake::Display::SwapBuffers() {
	glfwSwapBuffers(m_window);
}
