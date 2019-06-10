#include "monkeyShader.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

snake::MonkeyShader::MonkeyShader() : AdvancingShader("./res/shaders/monkeyShader") {
	m_uniforms.push_back(glGetUniformLocation(m_program, "MV"));
}