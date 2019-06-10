#include "caveShader.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

snake::CaveShader::CaveShader() : AdvancingShader("./res/shaders/caveShader") {
	m_uniforms.push_back(glGetUniformLocation(m_program, "MV"));
	m_uniforms.push_back(glGetUniformLocation(m_program, "timeStamp"));
}

void snake::CaveShader::Update(const glm::mat4 & projection,
	const glm::mat4 & MV,
	const glm::vec3& camPos,
	float timeStamp) const {
	glUseProgram(m_program);
	glUniform3fv(m_uniforms[0], 1, &camPos[0]);
	glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_uniforms[2], 1, GL_FALSE, &MV[0][0]);
	glUniform1f(m_uniforms[3], timeStamp);
}
