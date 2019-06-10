#include <iostream>
#include <fstream>
#include <cstring>

#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.hpp"


namespace snake {
	static const char* attributes[] = {
		"position", "texCoord", "color", "normal", "weightPrev", "weightCur", "weightNext"
	};
}

snake::Shader::Shader(const std::string& fileName):
	m_shaders(), m_uniforms() {

	m_program = glCreateProgram();
	m_shaders = { CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER),
		CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER) };
	
	for (uint m_shader : m_shaders)
		glAttachShader(m_program, m_shader);

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
}

snake::Shader::~Shader() {
	for (uint m_shader : m_shaders) {
		glDetachShader(m_program, m_shader);
		glDeleteShader(m_shader);
	}
	glDeleteProgram(m_program);
}

int snake::Shader::GetAttribIndex(Attributes attribute) const {
	return glGetAttribLocation(m_program, attributes[attribute]);
}

std::string snake::Shader::LoadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	
	else
		std::cerr << "Unable to load shader: " << fileName << std::endl;

	file.close();

	return output;
}

void snake::Shader::CheckShaderError(uint shader, uint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0U;
	uint sizeError = 1024U;

	std::vector<char> error(sizeError, (char)0);

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE) {
		if (isProgram)
			glGetProgramInfoLog(shader, sizeError, NULL, error.data());
		else
			glGetShaderInfoLog(shader, sizeError, NULL, error.data());

		std::cerr << errorMessage << ": '" << error.data() << "'" << std::endl;
	}
}

uint snake::Shader::CreateShader(const std::string& text, uint type) {
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}

snake::AdvancingShader::AdvancingShader(const char* fileName): Shader(fileName) {
	m_uniforms.push_back(glGetUniformLocation(m_program, "camPos"));
	m_uniforms.push_back(glGetUniformLocation(m_program, "projection"));
}

void snake::AdvancingShader::Update(const glm::mat4 & projection,
	const glm::mat4 & MV, const glm::vec3& camPos) const {
	glUseProgram(m_program);
	glUniform3fv(m_uniforms[0], 1, &camPos[0]);
	glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_uniforms[2], 1, GL_FALSE, &MV[0][0]);
}