#include <iostream> 

#include "snakeMesh.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

namespace snake {
	const uint snakeBuffers = 8U;
}

snake::SnakeMesh::SnakeMesh(const IndexedModel& model,
	const IndexedModel& simpleModel,
	const Texture* texture,
	const PickingShader& pickingShader,
	const SnakeShader& shader) :
	//Mesh(model, simpleModel, texture, pickingShader, 8U),
	shader(shader), texture(texture), pickingShader(pickingShader),
	numBuffers(snakeBuffers), m_vertexArrayBuffers(snakeBuffers, 0U),
	m_numIndices(model.indices.size()), collider(simpleModel) {

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

	std::vector<float> weights;
	
	if ((i = shader.GetAttribIndex(WEIGHT_PREV)) > -1) {
		model.GetPrevs(weights);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(weights[0]) * weights.size(), &weights[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 1, GL_FLOAT, GL_FALSE, 0, 0);
	}
	
	if ((i = shader.GetAttribIndex(WEIGHT_CUR)) > -1) {
		model.GetCurs(weights);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(weights[0]) * weights.size(), &weights[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 1, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if ((i = shader.GetAttribIndex(WEIGHT_NEXT)) > -1) {
		model.GetNexts(weights);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(weights[0]) * weights.size(), &weights[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 1, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[numBuffers - 1U]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

snake::SnakeMesh::~SnakeMesh() {
	glDeleteBuffers(numBuffers, m_vertexArrayBuffers.data());
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void snake::SnakeMesh::DrawForPicking(const glm::mat4& mvp, uint pickingColor) const {
	pickingShader.Update(mvp, pickingColor);
	Draw();
}

void snake::SnakeMesh::Draw() const {
	texture->Bind();
	glBindVertexArray(m_vertexArrayObject);
	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	glBindVertexArray(0);
}


void snake::SnakeMesh::Draw(const glm::mat4 & projection,
	const glm::mat4 & prevLinkMV,
	const glm::mat4 & curLinkMV,
	const glm::mat4 & nextLinkMV,
	const glm::vec3& camPos) const {
	shader.Update(projection, prevLinkMV, curLinkMV, nextLinkMV, camPos);
	Draw();
}
