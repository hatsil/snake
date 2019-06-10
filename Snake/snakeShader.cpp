#define GLEW_STATIC
#include <GL/glew.h>

#include "snakeShader.hpp"

snake::SnakeShader::SnakeShader(): AdvancingShader("./res/shaders/snakeShader") {
	//wheights:
	//glBindAttribLocation(m_program, 4, "a_weightPrev");
	//glBindAttribLocation(m_program, 5, "a_weightCur");
	//glBindAttribLocation(m_program, 6, "a_weightNext");

	m_uniforms.push_back(glGetUniformLocation(m_program, "linksMV.prev"));
	m_uniforms.push_back(glGetUniformLocation(m_program, "linksMV.cur"));
	m_uniforms.push_back(glGetUniformLocation(m_program, "linksMV.next"));
}

void snake::SnakeShader::Update(const glm::mat4 & projection,
	const glm::mat4 & prevLinkMV,
	const glm::mat4 & curLinkMV,
	const glm::mat4 & nextLinkMV,
	const glm::vec3 & camPos) const {
	glUseProgram(m_program);
	glUniform3fv(m_uniforms[0], 1, &camPos[0]);
	glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_uniforms[2], 1, GL_FALSE, &prevLinkMV[0][0]);
	glUniformMatrix4fv(m_uniforms[3], 1, GL_FALSE, &curLinkMV[0][0]);
	glUniformMatrix4fv(m_uniforms[4], 1, GL_FALSE, &nextLinkMV[0][0]);
}
