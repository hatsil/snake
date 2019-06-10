#include <iostream>
#include "display.hpp"
#include "inputManager.hpp"
#include <Windows.h>

extern bool pressed;

void init()
{
	glfwSetKeyCallback(display->m_window, key_callback);
	glfwSetWindowSizeCallback(display->m_window, window_size_callback);
}

int main() {
	display = new snake::Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	scn = new snake::Scene();
	init();
	//glfwSetInputMode(display.m_window,GLFW_STICKY_MOUSE_BUTTONS,1);

	while (!glfwWindowShouldClose(display->m_window)) {
		Sleep(30);
		scn->CalcActions();
		display->Clear(0.f, 0.5f, 1.0f, 1.0f);
		if (display->IsFullscreen()) {
			GLint viewport[4];
			glfwGetFramebufferSize(display->m_window, &viewport[2], &viewport[3]);
			window_size_callback(display->m_window, viewport[2], viewport[3]);
		}
		scn->Draw();
		
		display->SwapBuffers();
		glfwPollEvents();
	}

	delete scn;
	delete display;

	return 0;
}
