#pragma once

#include <GLFW/glfw3.h>
#include "scene.hpp"

const int DISPLAY_WIDTH = 1200;
const int DISPLAY_HEIGHT = 800;
const float FAR = 100.0f;
const float NEAR = 1.0f;
const float CAM_ANGLE = 60.0f;
float relation = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;

snake::Display* display;

snake::Scene* scn;

float factor = 1.0;

double x1 = 0, x2 = 0;
double ys1 = 0, y2 = 0;
float depth;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_RIGHT:
			scn->MoveRight();
			break;
		case GLFW_KEY_LEFT:
			scn->MoveLeft();
			break;
		case GLFW_KEY_UP:
			scn->MoveUp();
			break;
		case GLFW_KEY_DOWN:
			scn->MoveDown();
			break;
		case GLFW_KEY_SPACE:
			scn->SwitchCamera();
			break;
		}
	}
}

void updatePosition(double xpos, double ypos)
{
	x1 = x2;
	x2 = xpos;
	ys1 = y2;
	y2 = ypos;
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	scn->Resize(width, height);
	relation = (float)width / (float)height;
}