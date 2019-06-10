#define GLEW_STATIC
#include <GL/glew.h>

#include "pickingShader.hpp"

snake::PickingShader::PickingShader(): Shader("./res/shaders/pickingShader") {
	glBindAttribLocation(m_program, 0, "position");
	m_uniforms.push_back(glGetUniformLocation(m_program, "mvp"));
	m_uniforms.push_back(glGetUniformLocation(m_program, "pickingColor"));
}

void snake::PickingShader::Update(const glm::mat4& mvp, uint pickingColor) const {
	float b = ((pickingColor & 0x000000FF) >> 0) / 255.0f;
	float g = ((pickingColor & 0x0000FF00) >> 8) / 255.0f;
	float r = ((pickingColor & 0x00FF0000) >> 16) / 255.0f;

	glUseProgram(m_program);

	glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &mvp[0][0]);
	glUniform3f(m_uniforms[1], r, g, b);
}

