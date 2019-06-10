#include "mesh.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

snake::Mesh::Mesh(const IndexedModel& model,
	const IndexedModel& simpleModel,
	const Texture* texture,
	const Shader& shader,
	const PickingShader& pickingShader,
	uint numBuffers) :
	collider(simpleModel),
	texture(texture), pickingShader(pickingShader),
	m_vertexArrayBuffers(numBuffers, 0U),
	m_numIndices(model.indices.size()),
	numBuffers(numBuffers) {

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(numBuffers, m_vertexArrayBuffers.data());

	int i;
	if ((i = shader.GetAttribIndex(POSITION)) > -1) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if ((i = shader.GetAttribIndex(TEXCOORD)) > -1) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if ((i = shader.GetAttribIndex(NORMAL)) > -1) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if ((i = shader.GetAttribIndex(COLOR)) > -1) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(model.colors[0]) * model.colors.size(), &model.colors[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[numBuffers - 1U]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

snake::Mesh::~Mesh() {
	glDeleteBuffers(numBuffers, m_vertexArrayBuffers.data());
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void snake::Mesh::DrawForPicking(const glm::mat4& mvp, uint pickingColor) const {
	pickingShader.Update(mvp, pickingColor);
	Draw();
}

void snake::Mesh::Draw() const {
	if (texture)
		texture->Bind();
	glBindVertexArray(m_vertexArrayObject);
	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	glBindVertexArray(0);
}