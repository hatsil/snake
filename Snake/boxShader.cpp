#include "boxShader.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

snake::BoxShader::BoxShader() : AdvancingShader("./res/shaders/boxShader") {
	m_uniforms.push_back(glGetUniformLocation(m_program, "MV"));
}